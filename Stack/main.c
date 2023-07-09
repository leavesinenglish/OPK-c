#include <string.h>
#include <assert.h>
#include "stack.h"
int main() {
    {
        Stack test_stack;
        stack_create(&test_stack);
        stack_push(&test_stack, "abc");
        stack_push(&test_stack, "def");
        stack_push(&test_stack, "ghi");
        assert(strcmp(stack_peek(&test_stack), "ghi") == 0);
        stack_destroy(&test_stack);
    }
    {
        Stack test_stack;
        stack_create(&test_stack);
        stack_push(&test_stack, (Pointer) 58);
        assert((int) stack_peek(&test_stack) == 58);
        stack_destroy(&test_stack);
    }
    {
        Stack test_stack;
        stack_create(&test_stack);
        stack_push(&test_stack, "abc");
        stack_push(&test_stack, "def");
        stack_push(&test_stack, "ggg");
        stack_pop(&test_stack);
        assert(strcmp(stack_peek(&test_stack), "def") == 0);
        assert(stack_size(&test_stack) == 2);
        stack_destroy(&test_stack);
    }
    {
        Stack test_stack;
        stack_create(&test_stack);
        stack_push(&test_stack, "abc");
        stack_push(&test_stack, "def");
        stack_push(&test_stack, "ggg");
        assert(stack_size(&test_stack) == 3);
        stack_destroy(&test_stack);
        assert(stack_size(&test_stack) == -1);
    }
    {
        Stack test_stack;
        stack_create(&test_stack);
        for (int i = 0; i <= 1000; i++) {
            stack_push(&test_stack, (void *) (i * i));
        }
        for (int i = 0; i <= 500; i++) {
            stack_pop(&test_stack);
        }
        for (int i = 0; i <= 1000; i++) {
            stack_push(&test_stack, (void *) (i * i));
        }
        assert ((int) stack_peek(&test_stack) == 1000000);
        for (int i = 0; i <= 1500; i++) {
            stack_pop(&test_stack);
        }
        assert (stack_size(&test_stack) == 0);
    }
    return 0;
}