#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

double v[10000007];
double aux[10000007];

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
// Referência: 6.2.1.1 Parallel Task
// Paralelismo de tarefas utilizando OpenMP - Bianchini, Valoboas, Castro
#pragma omp task final((pivot - low) < n_threads) mergeable default(none) \
    shared(arr) firstprivate(low, pivot)
        quicksort(arr, low, pivot - 1);

#pragma omp task final((high - pivot) < n_threads) mergeable default(none) \
    shared(arr) firstprivate(high, pivot)
        quicksort(arr, pivot + 1, high);
    }
}

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

int read_file(double *vetor, const char *filename) {
    int index = 0;
    FILE *filep;
    char filaname_in[1000];
    strcpy(filaname_in, "entradas/");
    strcat(filaname_in, filename);
    if ((filep = fopen(filaname_in, "r")) == NULL) {
        printf("Failed to open file!\n");
        // return 1;
    } else {
        while ((fscanf(filep, "%lf", &vetor[index++])) != EOF) {
        }
    }
    return index - 1;
}

int main(int const argc, char const *argv[]) {
    const char *filename = argc > 1 ? argv[1] : "1000000.txt";
    n_threads = (argc > 2) ? atoi(argv[2]) : 4;
    omp_set_num_threads(n_threads);
    int n = read_file(v, filename);

    // Começando a medição do tempo
    double start_time = omp_get_wtime();

#pragma omp parallel
#pragma omp single
    quicksort(v, 0, n - 1);
    double ma = v[0], dp, mg = v[0], md, p95, minimo = v[0], maximo = v[n - 1];
    for (int i = 1; i < n; i++) {
        double x = v[i];
        ma += x;
        mg *= pow(x, 1.0 / n);
    }
    ma /= n;
    md = v[n / 2 - 1];
    p95 = v[(95 * (n + 1)) / 100 - 1];
    double aux_dp = 0.0;
    for (int i = 0; i < n; i++) {
        double aux_x = v[i] - ma;
        aux_dp += (aux_x * aux_x) / n;
    }
    dp = sqrt(aux_dp);

    double end_time = omp_get_wtime();

    FILE *filep;
    char filename_out[1000];
    strcpy(filename_out, "saidas/");
    strcat(filename_out, filename);
    filep = fopen(filename_out, "a");
    fprintf(filep, "%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", filename,
            ma, dp, mg, md, p95, minimo, maximo);
    printf("%s - tempo: %.6lfs\n", filename, end_time - start_time);
    return 0;
}
