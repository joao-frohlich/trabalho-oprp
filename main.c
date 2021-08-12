#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

double v[1000005];
double aux[1000005];

void mergesort(double *v, int l, int r) {
    if (l != r) {
        int m = l + (r - l) / 2;
        mergesort(v, m + 1, r);
        mergesort(v, l, m);
        int i = l, j = m + 1, k = 0;
        while (i <= m || j <= r) {
            if (i > m)
                aux[k++] = v[j++];
            else if (j > r)
                aux[k++] = v[i++];
            else {
                if (v[i] < v[j])
                    aux[k++] = v[i++];
                else
                    aux[k++] = v[j++];
            }
        }
        for (i = l, k = 0; i <= r; i++, k++) v[i] = aux[k];
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
    int n = read_file(v, filename);

    // Começando a medição do tempo
    double start_time = wtime();

    mergesort(v, 0, n - 1);
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

    double end_time = wtime();

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
