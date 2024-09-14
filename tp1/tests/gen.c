#include "../src/graph.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

edge* get_edge(graph* g, vertex v, vertex u) {
    list* edges = graph_list_edges_of(g, v);
    list_foreach(edges, edge, e) {
        if(e->u == u) return e;
    }
    return NULL;
}

void force_connections(graph* g, int N) {
    list* search_queue = list_new(sizeof(vertex));
    vertex v = 0;
    list_push(search_queue, &v);
    int visited[N];
    memset(visited, 0, sizeof(visited));
    while(search_queue->len > 0) {
        v = *(vertex*)list_get(search_queue, search_queue->len-1);
        list_remove(search_queue, search_queue->len-1);
        if(v == N-1) break;
        if(visited[v]) continue;

        visited[v] = 1;

        list* children = graph_list_edges_of(g, v);
        list_foreach(children, edge, e) {
            list_push(search_queue, &e->u);
        }
    }

    // Caso o último vértice não tenha sido atingido, adicionamos arestas extras
    // para garantir.
    for(int i = N-1; i > 0; i--) {
        if(visited[i]) break;
        graph_add_edge(g, i-1, i, rand() % 10000);
    }

    list_free(search_queue);
}

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "Esperava apenas 2 argumentos\n");
        return 1;
    }
    int N;
    sscanf(argv[1], "%d", &N);
    FILE* outfile = fopen(argv[2], "w");
    srand(time(NULL));
    graph* g = graph_new(N);
    for(int i = 0; i < N; i++) {
        graph_add_vertex(g);
    }
    int step = ceil(N/100.0);
    int M = 0;
    for(vertex v = 0; v < N-1; v++) {
        int count = rand() % 10;
        for(int i = 0; i < count; i++) {
            int u = rand() % (v + step);
            weight w = v;
            if(u > v) w *= 4;
            else w /= 4;
            graph_add_edge(g, v, v+u, w);
            M++;
        }
        graph_add_edge(g, v, v+1, v*2);
        M++;
    }

    // force_connections(g, N);

    fprintf(outfile, "%d %d %d\n", N, M, 10);
    for(int i = 0; i < N; i ++) {
        list* children = graph_list_edges_of(g, i);
        list_foreach(children, edge, e) {
            fprintf(outfile, "%d %d %ld\n", e->v+1, e->u+1, e->weight);
        }
    }

    graph_free(g);
    fclose(outfile);
}
