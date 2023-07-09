#ifndef STACK_STACK_H
#define STACK_STACK_H
typedef void *Pointer;
typedef struct tStack{
    unsigned lenght;
    unsigned capacity;
    Pointer *arr;
} Stack;
void stack_create(Stack *s);
void stack_destroy(Stack *s);
size_t stack_size(Stack *s);
void stack_push(Stack *s, void *elem);
Pointer stack_pop(Stack *s);
Pointer stack_peek(Stack *s);
#endif
