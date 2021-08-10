#include <bits/stdc++.h>
using namespace std;

int main(int argc, char const *argv[]) {
    int n = 20;
    vector<double> v(n);
    double aux[n] = {2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.2};
    for (int i = 0; i < n; i++){
        v[i] = aux[i];
    }
    double ma = v[0], dp, mg = v[0], md, p95, minimo = v[0], maximo = v[0];
    int qtd_elem = 1;
    for (int i = 1; i < n; i++){
        double x = v[i];
        ma += x;
        mg *= x;
        minimo = (x < minimo) ? x : minimo;
        maximo = (x > maximo) ? x : maximo;
        qtd_elem++;
    }
    ma /= qtd_elem;
    mg = pow(mg, 1.0/qtd_elem);
    sort(v.begin(), v.end()); //Precisa implementar a própria função de ordenação
    md = v[qtd_elem/2-1];
    p95 = v[(95*(qtd_elem+1))/100-1];
    double aux_dp = 0.0;
    for (int i = 0; i < qtd_elem; i++){
        double aux_x = v[i]-ma;
        aux_dp += (aux_x*aux_x);
    }
    dp = sqrt(aux_dp/qtd_elem);
    cout << "ma " << ma << "\tdp " << dp << "\tmg " << mg << "\tmd " << md << "\tp95 " << p95 << "\tmin " << minimo << "\tmax " << maximo << endl;
    return 0;
}

//Valores de referência
//ma 4.235	dp 1.41396	mg 3.97701	md 4	p95 6.2	min 2	max 6.5
