#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

double vetor[16777216];
double vetor_log[16777216];
int n_threads;

void swap(double *a, double *b) {
    double t = *a;
    *a = *b;
    *b = t;
}

int divide(double arr[], int low, int high) {
    double pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
        if (arr[j] < pivot) swap(&arr[++i], &arr[j]);
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(double arr[], int low, int high) {
    if (low < high) {
        int pivot = divide(arr, low, high);
#pragma omp task final((pivot - low) < n_threads) mergeable default(none) \
    shared(arr) firstprivate(low, pivot)
        quicksort(arr, low, pivot - 1);

#pragma omp task final((high - pivot) < n_threads) mergeable default(none) \
    shared(arr) firstprivate(high, pivot)
        quicksort(arr, pivot + 1, high);
    }
}

int open_file(int idx, char *files[]) {
    FILE *filep;
    char filename[100];
    strcpy(filename, "entradas/");
    strcat(filename, files[idx]);
    filep = fopen(filename, "r");
    int index = 0;
    if (filep == NULL) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    } else {
        while ((fscanf(filep, "%lf", &vetor[index++])) != EOF) {
            vetor_log[index - 1] = log(vetor[index - 1]);
        }
    }
    fclose(filep);
    return index - 1;
}

// cuda sum
static const int blockSize = 1024;
static const int gridSize = 24;

__device__ double sum_warp(volatile double *shArr) {
    int idx = threadIdx.x % warpSize;  // quantidade de wrap permitido pela GPU
    if (idx < 16) {
        shArr[idx] += shArr[idx + 16];
        shArr[idx] += shArr[idx + 8];
        shArr[idx] += shArr[idx + 4];
        shArr[idx] += shArr[idx + 2];
        shArr[idx] += shArr[idx + 1];
    }
    return shArr[0];
}

__global__ void sum_reduction(const double *a, const double arraySize,
                              double *out) {
    int idx = threadIdx.x;  // idx do bloco
    double sum = 0;
#pragma omp parallel for reduction(+ : sum)
    for (int i = idx; i < arraySize; i += blockSize) sum += a[i];
    __shared__ double r[blockSize];
    r[idx] = sum;
    sum_warp(&r[idx & ~(warpSize - 1)]);  // soma dos valores do bloco
    __syncthreads();                      // barreira
    if (idx < warpSize) {
        r[idx] = idx * warpSize < blockSize ? r[idx * warpSize]
                                            : 0;  // lidando com overflow
        sum_warp(r);                              // computando bloco do inicio
        if (idx == 0) *out = r[0];  // salvando na posicao 0 a soma
    }
}

__host__ double sumArray(double *arr, int size) {
    double *dev_arr;
    cudaMalloc((void **)&dev_arr, size * sizeof(double));
    cudaMemcpy(dev_arr, arr, size * sizeof(double), cudaMemcpyHostToDevice);

    double out;
    double *dev_out;
    cudaMalloc((void **)&dev_out, sizeof(double) * gridSize);

    int *dev_lastBlockCounter;
    cudaMalloc((void **)&dev_lastBlockCounter, sizeof(int));
    cudaMemset(dev_lastBlockCounter, 0, sizeof(int));

    sum_reduction<<<gridSize, blockSize>>>(dev_arr, size, dev_out);
    cudaDeviceSynchronize();  // barreira

    cudaMemcpy(&out, dev_out, sizeof(double), cudaMemcpyDeviceToHost);
    cudaFree(dev_arr);
    cudaFree(dev_out);
    return out;
}

void solve(int idx, char *files[], int size) {
    // sorting
#pragma omp parallel
#pragma omp single
    quicksort(vetor, 0, size - 1);
    // ===============

    // aritmetical and geometric mean
    double ma = 0, mg = 0;
    ma = sumArray(vetor, size) / (double)size;
    mg = sumArray(vetor_log, size) / (double)size;
    mg = exp(mg);
    // ===============

    // positional values
    double md = vetor[size / 2 - 1];
    double p95 = vetor[(95 * (size + 1)) / 100 - 1];
    double maximo = vetor[size - 1];
    double minimo = vetor[0];
    // ===============

    double aux_dp = 0.0;
    int i;
#pragma omp parallel for reduction(+ : aux_dp) schedule(dynamic) \
    private(i) shared(vetor, size)
    for (i = 0; i < size; i++) {
        double aux_x = vetor[i] - ma;
        aux_dp += (aux_x * aux_x);
    }
    double dp = sqrt(aux_dp / size);

    FILE *filep;
    filep = fopen("saidas/results", "a");
    fprintf(filep, "%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", files[idx],
            ma, dp, mg, md, p95, minimo, maximo);
    fclose(filep);
}

int main(int argc, char *argv[]) {
    int i, n;

    n_threads = omp_get_max_threads();
    omp_set_num_threads(n_threads);

    double start_time_total = omp_get_wtime();

    for (i = 1; i < argc; i++) {
        n = open_file(i, argv);
        double start_time = omp_get_wtime();
        solve(i, argv, n);
        double end_time = omp_get_wtime();
        printf("%s - tempo: %.6lfs\n", argv[i], end_time - start_time);
    }

    double end_time_total = omp_get_wtime();

    printf("Tempo total: %.6lfs\n", end_time_total - start_time_total);

    return 0;
}
