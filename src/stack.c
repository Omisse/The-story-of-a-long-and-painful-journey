#include <stdlib.h>

#define SZ 15

typedef struct stack {
    double *mass;
    int size;
    int top;
} stack;

stack *init() {
    stack *ex = NULL;
    ex = malloc(sizeof(stack));
    if (ex == NULL) return NULL;
    ex->size = SZ;
    ex->mass = malloc(ex->size * sizeof(double));
    if (ex->mass == NULL) {
        free(ex);
        return NULL;
    }
    ex->top = 0;
    return ex;
}

int new_size(stack *stack) {
    stack->size += 5;
    stack->mass = realloc(stack->mass, stack->size * sizeof(double));
    if (stack->mass == NULL) return -1;
    return 0;
}

void push(stack *stack, double elem) {
    if (stack->top >= stack->size) {
        new_size(stack);
    }
    stack->mass[stack->top] = elem;
    stack->top++;
}

double pop(stack *stack) {
    double val = 0;
    if (stack->top == 0) { val = 1.0/0.0; }
    else {
        stack->top--;
        val = stack->mass[stack->top];
    }
    return val;
}

void destroy(stack **stack) {
    free((*stack)->mass);
    free(*stack);
    *stack = NULL;
}
