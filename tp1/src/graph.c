#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "heap.h"

typedef struct vertex_data vertex_data;


struct graph {
    list* vertices;
};

struct vertex_data {
    list* edges;
    vertex id;
};

struct cslot {
    weight w;
    char c;
};


struct tp_info parse_info(FILE* input_file) {
    int m, n, k;
    bool letters = false;
    char* line = malloc(256);

    fgets(line, 255, input_file);
    if(strstr(line, "LETTER") != NULL) {
        letters = true;
        fgets(line, 255, input_file);
    }
    sscanf(line, "%d %d %d\n", &n, &m, &k);
    graph* g = graph_new(n);

    list* cities_list = list_new(sizeof(vertex));

    for(int i = 0; i < n; i++) {
        vertex v = graph_add_vertex(g);
        list_push(cities_list, &v);
    }

    vertex* cities = cities_list->elements;
    for(int i = 0; i < m; i++) {
        int u = 0, v = 0;
        weight w = 0;
        do
            if(fgets(line, 255, input_file) == NULL) goto end;
        while(strcspn(line, "\n") == 0);
        if(!letters) {
            sscanf(line, "%d %d %lu\n", &v, &u, &w);
            v = cities[v-1];
            u = cities[u-1];
        } else {
            sscanf(line, "%c %c %lu\n", (char*)&v, (char*)&u, &w);
            v -= 'a';
            u -= 'a';
        }
        graph_add_edge(g, v, u, w);
    }
    end:;

    int start = cities[0];
    int end = cities[n-1];

    free(line);
    list_free(cities_list);
    return (struct tp_info){
        .g=g,
        .k=k,
        .start=start,
        .end = end,
    };
}


static vertex_data vdata_new(vertex id) {
    vertex_data vdata = {.edges = list_new(sizeof(edge)), .id=id};
    return vdata;
}

graph* graph_new(size_t vertex_capacity) {
    graph* g = malloc(sizeof(graph));
    g->vertices = list_new_with_capacity(sizeof(vertex_data), vertex_capacity);
    return g;
}

vertex graph_add_vertex(graph* g) {
    vertex v = g->vertices->len;
    vertex_data vdata = vdata_new(v);
    list_push(g->vertices, &vdata);
    return v; 
}

vertex_data* graph_get_vertex_data(graph* g, vertex v) {
    return list_get(g->vertices, v);
}

bool graph_add_edge(graph* g, vertex v, vertex u, weight w) {
    vertex_data* vdata = graph_get_vertex_data(g, v);
    vertex_data* udata = graph_get_vertex_data(g, u);
    if (!vdata || !udata) {
        return false;
    }
    edge e = (edge){.v = v, .u = u, .weight = w};
    list_push(vdata->edges, &e);
    return true;
}

list* graph_list_edges_of(graph* g, vertex v) {
    return graph_get_vertex_data(g, v)->edges;
}

list* graph_shortest_paths(graph* g, int k, vertex source, vertex dest) {
    const size_t vertex_count = g->vertices->len;

    int visit_count[vertex_count];
    for(int i = 0; i < vertex_count; i++) {
        visit_count[i] = 0;
    }

    heap* queue = heap_new(sizeof(vertex));
    heap_push(queue, &source, 0);


    list* paths = list_new(sizeof(list*));
    while(visit_count[dest] < k) {
        // Pega o nó com distancia minima
        vertex u;
        weight w;
        // Heap vazia, acabou o algoritmo
        // O(1)
        if(!heap_pop(queue, &u, &w)) break;
        if(visit_count[u] == k) continue;
        // Marca o nó u como visitado denovo
        visit_count[u] += 1;
        if(u == dest) {
            list_push(paths, &w);
        }

        // Coloca todos os filhos de `u` e coloca na heap
        vertex_data* udata = graph_get_vertex_data(g, u);
        list_foreach(udata->edges, edge, e) {
            vertex v = e->u;
            heap_push(queue, &v, e->weight + w);
        }
    }

    heap_free(queue);

    return paths;
}

void graph_free(graph* g) {
    list_foreach(g->vertices, vertex_data, vdata) {
        list_free(vdata->edges);
    }
    list_free(g->vertices);
    free(g);
}



