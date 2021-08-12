# Trabalho Final OPRP

## Escopo

Computar os seguintes dados estátisticos das bases fornecidas

- média aritmética (MA) - Soma de todos os valores dividido pela quantidade de valores
- desvio padrão (DP) - Raiz quadrada do ((somatório do quadrado da diferença entre todos os valores e a média aritmética) dividido pelo número de elementos)
- média geométrica (MG) - Raíz n-ésima da multiplicação de n valores
- mediana (MD) - Valor do elemento médio de um conjunto de valores ordenados
- 95 percentil (95p) - Elemento localizado em 95% da distribuição de elementos
- menor valor (min) - Menor valor de um conjunto de elementos
- maior valor (max) - Maior valor de um conjunto de elementos

## Executando

Para compilar com mais facilidade, foi escrito um `Makefile`

```bash
make
```

Será criado uma pasta de nome `saidas`, onde os dados calculados serão armazenados.

Para testar, abasteca a pasta `entradas` com os arquivos casos de teste e execute com o seguinte comando

```bash
./main N.txt
```

> Sendo N o tamanho da base de testes
