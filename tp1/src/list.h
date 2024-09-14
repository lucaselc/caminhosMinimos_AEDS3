#pragma once

#include <stddef.h>
#include <stdbool.h>

// Uma lista dinâmica de elementos de tamanho fixo.
typedef struct {
    void* elements;
    size_t element_size;
    size_t len;
    size_t cap;
} list;

// Cria e retorna uma lista.
list* list_new(size_t element_size);
list* list_new_with_capacity(size_t element_size, size_t capacity);

void list_push(list* l, void* element);
bool list_insert(list* l, void* element, size_t index);
// Retorna o elemento no índice `index`. Caso o índice esteja fora da lista, retorna `NULL`.
void* list_get(list* l, size_t index);
// Retorna o elemento no índice `index` sem checar se está fora da lista
void* list_get_unchecked(list* l, size_t index);

bool list_remove(list* l, size_t index);

typedef int (*element_comparator_fn)(const void*, const void*);
void list_sort(list* l, element_comparator_fn comparator);
bool list_eq(list* l1, list* l2, element_comparator_fn comparator);
void* list_min(list* l, element_comparator_fn comparator);

#define list_foreach(l, type, var)\
    for(type* var = (l)->elements;\
        var < (type*)((l)->elements + (l)->len * (l)->element_size);\
        var = ((void*) var + (l)->element_size))

void list_free(list* l);
