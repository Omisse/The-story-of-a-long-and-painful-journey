#ifndef STACK
#define STACK
typedef struct stack {
    double *mass;
    int size;
    int top;
} stack;

stack *init();
int new_size(stack *stack);
void push(stack *stack, double elem);
double pop(stack *stack);
void destroy(stack **stack);
#endif
