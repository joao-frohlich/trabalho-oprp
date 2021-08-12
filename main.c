#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

double v[1000005];
double aux[1000005];

void mergesort(double *v, int l, int r){
    if (l != r) {
        int m = l+(r-l)/2;
        mergesort(v,m+1,r);
        mergesort(v,l,m);
        int i = l, j = m+1, k = 0;
        while (i <= m || j <= r){
            if (i > m){
                aux[k++] = v[j++];
            } else if (j > r) {
                aux[k++] = v[i++];
            } else {
                if (v[i] < v[j]){
                    aux[k++] = v[i++];
                } else {
                    aux[k++] = v[j++];
                }
            }
        }
        for (i = l, k = 0; i <= r; i++, k++){
            v[i] = aux[k];
        }
    }
}

double wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

int read_file(double *vetor){
    FILE* filep;
    int index = 0;
    if((filep = fopen("entrada/1000000.txt", "r")) == NULL) {
        printf("Failed to open file!\n");
        // return 1;
    } else {
        while((fscanf(filep, "%lf", &vetor[index++])) != EOF) {}
    }
    return index-1;
}

int main(int argc, char const *argv[]) {
    int n = read_file(v);

    //Começando a medição do tempo
    double start_time = wtime();

    mergesort(v,0,n-1);
    double ma = v[0], dp, mg = v[0], md, p95, minimo = v[0], maximo = v[n-1];
    for (int i = 1; i < n; i++){
        double x = v[i];
        ma += x;
        mg *= pow(x, 1.0/n);
    }
    ma /= n;
    md = v[n/2-1];
    p95 = v[(95*(n+1))/100-1];
    double aux_dp = 0.0;
    for (int i = 0; i < n; i++){
        double aux_x = v[i]-ma;
        aux_dp += (aux_x*aux_x)/n;
    }
    dp = sqrt(aux_dp);


    double end_time = wtime();
    printf("ma   = %.6lf\ndp   = %.6lf\nmg   = %.6lf\nmd   = %.6lf\np95  = %.6lf\nmin  = %.6lf\nmax  = %.6lf\n", ma, dp, mg, md, p95, minimo, maximo);
    printf("Tempo: %.6lf\n", end_time-start_time);
    return 0;
}

//Valores de referência
//ma 4.235	dp 1.41396	mg 3.97701	md 4	p95 6.2	min 2	max 6.5
