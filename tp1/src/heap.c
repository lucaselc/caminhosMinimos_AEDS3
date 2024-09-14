#include "heap.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    weight w;
    char ptr[];
} slot;

struct heap {
    list list;
};


// Cria e retorna uma fila de prioridade.
heap* heap_new(size_t element_size) {
    return (heap*)list_new(element_size+sizeof(slot));
}

static void memswap(void *a, void *b, size_t element_size) {
    char *temp = malloc(element_size);
    memcpy(temp, a, element_size);
    memcpy(a, b, element_size);
    memcpy(b, temp, element_size);
    free(temp);
}

// 
void heap_push(heap* h, void* element, weight w) {
    const size_t element_size = h->list.element_size;

    slot *new_slot = malloc(element_size);
    memcpy(&new_slot->ptr, element, element_size-sizeof(slot));
    new_slot->w = w;

    size_t i = h->list.len;
    list_push(&h->list, new_slot);
    for(;;) {
        size_t parent_id = (i-1)/2;
        slot* s = list_get(&h->list, i);
        slot* parent = list_get(&h->list, parent_id);
        if(parent && s && s->w < parent->w) {
            memswap(s, parent, h->list.element_size);
            i = parent_id;
        } else {
            break;
        }
    }

    free(new_slot);
}

bool heap_pop(heap* h, void* element, weight* w) {
    const size_t element_size = h->list.element_size;

    slot *first = list_get(&h->list, 0);

    // Se não há elementos ou elementos preenchidos, retornamos false.
    if(h->list.len == 0 || first->w == NO_WEIGHT) {
        *w = NO_WEIGHT;
        return false;
    }

    // Copiar elemento e peso para os argumentos
    memcpy(element, first->ptr, element_size-sizeof(slot));
    *w = first->w;

    // Invalidar slot
    first->w = NO_WEIGHT;

    size_t i = 0;
    slot* parent;
    while((parent = list_get(&h->list, i))) {
        // O nodo atual não existe ou está vazio
        if(parent == NULL) break;

        slot *left = list_get(&h->list, i*2+1);
        slot *right = list_get(&h->list, i*2+2);

        slot *next = parent;

        int nexti = i;
        if(left && left->w < next->w) {
            next = left;
            nexti = i*2+1;
        }
        if (right && right->w < next->w) {
            next = right;
            nexti = i*2+2;
        }

        if (next == parent) break;

        // Trocamos o menor dos filhos com o pai
        // e depois, repetimos o processo para o filho;
        memswap(parent, next, h->list.element_size);
        i = nexti;
    }
    return true;
}


void heap_free(heap* h) {
    list_free(&h->list);
}
