# Descrição dessa pasta
Os arquivos aqui foram utilizados para gerar os gráficos da nossa documentação. É possível gerar vários arquivos de entrada para o programa utilizando o programa `gen`.

O programa `runner` (compilado a partir so `src/main.c`) faz a mesma coisa que `tp1`, exceto que sua saída é reduzida e pensada em coleta de dados.

Como apenas geramos a pasta `worst` por último, o programa `gen` gera arquivos com o pior caso do algoritmo.

Um exemplo de uso para o `gen`:
```sh
./gen 10000 test_input-10000.txt
```

Esse arquivo conterá 10000 vértices e uma quantidade aleatória de arestas, mas que sempre será maior que a quantidade de vértice, por um fator de ~4.8x.

## Scripts
Os vários scripts dessa pasta foram usados para organizar arquivos e gerar `json`s com os dados para a geração dos gráficos. O código foi feito rapidamente com o intuito de ser utilizado apenas uma ou duas vezes, então por favor desconsidere todos os `.py` e `.sh`.

