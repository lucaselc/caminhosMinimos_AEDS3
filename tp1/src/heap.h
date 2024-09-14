
#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "graph.h"

#define NO_WEIGHT ((weight)~0)

// Uma fila de prioridade dinâmica.
typedef struct heap heap;

// Cria e retorna uma fila de prioridade.
heap* heap_new(size_t element_size);
// Cria e retorna uma fila de prioridade a partir de um vetor;

void heap_push(heap* h, void* element, weight w);

[[nodiscard]]
bool heap_pop(heap* h, void* element, weight* w);

void heap_free(heap* h);
