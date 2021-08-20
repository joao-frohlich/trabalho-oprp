#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

double vetor[100000008];
int n_threads;
double maximo, minimo;

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
    maximo = 0xffffffff;
    maximo = -0xffffffff;
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
        }
        // ./main_openmp `< in`
    }
    fclose(filep);
    return index - 1;
}

void solve(int idx, char *files[], int size) {
#pragma omp parallel
#pragma omp single
    quicksort(vetor, 0, size - 1);
    double ma = 0, mg = 1, dp, md, p95;
    for (int i = 0; i < size; i++) {
        double x = vetor[i];
        ma += x;
        mg *= pow(x, 1.0 / size);
    }
    ma /= size;
    md = vetor[size / 2 - 1];
    p95 = vetor[(95 * (size + 1)) / 100 - 1];
    double aux_dp = 0.0;
    for (int i = 0; i < size; i++) {
        double aux_x = vetor[i] - ma;
        aux_dp += (aux_x * aux_x) / size;
    }
    dp = sqrt(aux_dp);
    maximo = vetor[size - 1];
    minimo = vetor[0];

    FILE *filep;
    filep = fopen("saidas/results", "a");
    fprintf(filep, "%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", files[idx],
            ma, dp, mg, md, p95, minimo, maximo);
    fclose(filep);
}

int main(int argc, char *argv[]) {
    int i, j;
    int n;
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
