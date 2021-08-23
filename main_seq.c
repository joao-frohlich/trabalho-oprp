#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

double vetor[100000008];

void swap(double *a, double *b) {
    double t = *a;
    *a = *b;
    *b = t;
}

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
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
        quicksort(arr, low, pivot - 1);
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
        }
    }
    fclose(filep);
    return index - 1;
}

void solve(int idx, char *files[], int size) {
    // sorting
    quicksort(vetor, 0, size - 1);
    // ===============
    // aritmetical and geometric mean
    double ma = 0, mg = 1;
    int i;

    for (i = 0; i < size; i++) {
        double x = vetor[i];
        ma += x;
        mg *= pow(x, 1.0 / size);
    }
    ma /= size;
    // ===============

    // positional values
    double md = vetor[size / 2 - 1];
    double p95 = vetor[(95 * (size + 1)) / 100 - 1];
    double maximo = vetor[size - 1];
    double minimo = vetor[0];
    // ===============

    double aux_dp = 0.0;
    for (int i = 0; i < size; i++) {
        double aux_x = vetor[i] - ma;
        aux_dp += (aux_x * aux_x);
    }
    double dp = sqrt(aux_dp / (double)size);

    FILE *filep;
    filep = fopen("saidas/results", "a");
    fprintf(filep, "%s %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf %.6lf\n", files[idx],
            ma, dp, mg, md, p95, minimo, maximo);
    fclose(filep);
}

int main(int argc, char *argv[]) {
    int i, n;

    double start_time_total = wtime();
    for (i = 1; i < argc; i++) {
        n = open_file(i, argv);
        double start_time = wtime();
        solve(i, argv, n);
        double end_time = wtime();
        printf("%s - tempo: %.6lfs\n", argv[i], end_time - start_time);
    }

    double end_time_total = wtime();

    printf("Tempo total: %.6lfs\n", end_time_total - start_time_total);

    return 0;
}
