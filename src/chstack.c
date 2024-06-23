#include "chstack.h"

#include <stdlib.h>
#include <string.h>

CharStack chs_init() {
    CharStack stack;
    stack.strings = (char**)malloc(sizeof(char*));
    stack.topIndex = -1;
    return stack;
}

int chs_is_empty(CharStack stack) { return stack.topIndex == -1; }

void chs_push(CharStack* stack, char* str) {
    stack->strings = realloc(stack->strings,sizeof(char*)*((stack->topIndex)+2));
    stack->topIndex += 1;
    stack->strings[stack->topIndex] = malloc((1+strlen(str))*sizeof(char));
    strcpy(stack->strings[stack->topIndex], str);
}

//returned char* needs to be manually freed
char* chs_pop(CharStack* stack) {
    char* result = NULL;
    if (!chs_is_empty(*stack)) {
        result = (stack)->strings[(stack)->topIndex];
        (stack)->strings = realloc((stack)->strings, sizeof(char*) * (stack->topIndex));
        (stack)->topIndex = (stack)->topIndex - 1;
    }
    return result;
}

char* chs_peek(CharStack stack) {
    char* result = NULL;
    if (!chs_is_empty(stack)) {
        result = stack.strings[stack.topIndex];
    }
    return result;
}

/*
int pop(CharStack** stack) {
    int result = -__INT_MAX__ - 1;
    if (!is_empty(**stack)) {
        result = (*stack)->values[(*stack)->topIndex];
        (*stack)->topIndex = (*stack)->topIndex - 1;
        (*stack)->values = (int*)realloc((*stack)->values, sizeof(int) * (((*stack)->topIndex) + 1));
        int* real_val = malloc(sizeof(int) * (((*stack)->topIndex) + 1)
    }
    return result;
}
*/
void chs_destroy(CharStack* stack) {
    while (!chs_is_empty(*stack)) {
        char* str = chs_pop(stack);
        free(str);
    }
    free(stack->strings);
    stack->strings = NULL;
}