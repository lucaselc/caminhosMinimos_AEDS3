#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "graph.h"
#include "list.h"
#include "timer.h"


int main(int argc, char** argv) {
    FILE* input_file = NULL;
    FILE* output_file = stderr;
    char c;
    while((c = getopt(argc, argv, "i:o:")) != -1) {
        switch (c) {
            case 'i':
                input_file = fopen(optarg, "r");
                if(!input_file) fprintf(stderr, "Falha ao abrir arquivo %s\n", optarg);
                break;
            case 'o':
                output_file = fopen(optarg, "w");
                if(!output_file) fprintf(stderr, "Falha ao abrir arquivo %s\n", optarg);
                break;
            default:
                ;;
        }
    }
    if(!(input_file && output_file)) {
        if (!input_file) 
            fputs("Faltando arquivo de entrada\n", stderr);
        if (!output_file) 
            fputs("Faltando arquivo de saida\n", stderr);
        return 1;
    }

    instant before = instant_now();
    struct tp_info info = parse_info(input_file);
    instant after = instant_now();
    #ifndef MACHINE
    printf("Tempo de IO:\n");
    #endif
    instant_print_elapsed(after, before);

    graph* g = info.g;

    before = instant_now();
    list* paths = graph_shortest_paths(g, info.k, info.start, info.end);
    after = instant_now();
    #ifndef MACHINE
    printf("Tempo de resolução:\n");
    #endif
    instant_print_elapsed(after, before);

    list_foreach(paths, weight, w) {
        fprintf(output_file, "%lu ", *w);
    }
    fputs("\n", output_file);

    list_free(paths);
    graph_free(g);
    fclose(input_file);
    fclose(output_file);
    return 0;
}
