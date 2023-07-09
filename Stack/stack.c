#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Создать пустой стек */
void stack_create(Stack *pstack){
    pstack->lenght = 0;
    pstack->capacity = 1;
    pstack->arr = malloc(pstack->capacity*sizeof(int));
    if (pstack->arr == NULL){
        printf("Memory error\n");
        exit(1);
    }
}

/* Уничтожить стек, освободив выделенную память */
void stack_destroy(Stack *pstack){
    free(pstack->arr);
    pstack->arr = NULL;
    pstack->lenght = -1;
    pstack->capacity = -1;
}

/* Поместить значение value на вершину стека */
void stack_push(Stack *pstack, Pointer value){
    if (pstack->lenght == pstack->capacity) {
        // extend stack
        unsigned newcapacity = pstack->capacity * 2;
        Pointer *newarr = realloc(pstack->arr,newcapacity * sizeof(Pointer));
        if (newarr == NULL) {
            printf("Memory error\n");
            exit(1);
        }
        pstack->arr = newarr;
        pstack->capacity = newcapacity;
    }
    // adding element
    pstack->arr[(pstack->lenght)++] = value;
}

/* Возвращает количество элементов в стеке (0, если стек пуст) */
size_t stack_size(Stack *pstack){
    return pstack->lenght;
}
/*
 * Возвращает значение с вершины стека, не удаляя его из стека.
 * Если стек пуст, возвращает 0
 */
Pointer stack_peek(Stack *pstack) {
    if (pstack->lenght == 0) return 0;
    return pstack->arr[pstack->lenght - 1];
}

/* Снять значение с вершины стека. Если стек пуст, возвращает 0 */
Pointer stack_pop(Stack *pstack) {
    if (pstack->lenght == 0) return 0;
    Pointer peek = stack_peek(pstack);
    pstack->arr[pstack->lenght - 1] = NULL;
    pstack->lenght--;
    // reduce stack
    if (pstack->lenght < pstack->capacity / 4) {
        unsigned newcapacity = pstack->capacity / 2;
        Pointer *newarr = realloc(pstack->arr,newcapacity * sizeof(Pointer));
        if (newarr == NULL) {
            printf("Memory error\n");
            exit(1);
        }
        pstack->arr = newarr;
        pstack->capacity = newcapacity;
    }
    return peek;
}
