#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

double polish_calc(char *str, double x) {
    // создание пустого стэка
    stack *stack;
    //инициализация стэка
    stack = init();
    double res, num = 0, temp;
    for (int i = 0; i < (int)strlen(str); i++) {
        if (('0' <= str[i]) && (str[i] <= '9')) {
            num = num * 10.0;
            num += ((int)str[i] - 48);
            if (str[i + 1] == ' ' || str[i + 1] == '\0' || str[i + 1] == '\n') {
                push(stack, num);
                num = 0;
            }
        } else if (str[i] == '+') {
            res = pop(stack) + pop(stack);
            push(stack, res);
        } else if (str[i] == '-') {
            res = 0 - pop(stack);
            temp = pop(stack);
            if (temp == -1) temp = 0;
            res += temp;
            push(stack, res);
        } else if (str[i] == '*') {
            res = pop(stack) * pop(stack);
            push(stack, res);
        } else if (str[i] == '/') {
            res = (1.0 / pop(stack)) * pop(stack);
            push(stack, res);
        } else if (str[i] == 's') {
            if (str[i + 1] == 'i') {
                i += 2;
                res = sin(pop(stack));
                push(stack, res);
            } else {
                i += 3;
                res = sqrt(pop(stack));
                push(stack, res);
            }
        } else if (str[i] == 'c') {
            if (str[i + 1] == 'o') {
                i += 2;
                res = cos(pop(stack));
                push(stack, res);
            } else {
                i += 2;
                res = 1.0 / tan(pop(stack));
                push(stack, res);
            }
        } else if (str[i] == 't') {
            i += 1;
            res = tan(pop(stack));
            push(stack, res);
        } else if (str[i] == 'l') {
            i += 1;
            res = log(pop(stack));
            push(stack, res);
        } else if (str[i] == 'x')
            push(stack, x);
    }
    res = pop(stack);
    //очистка стека
    destroy(&stack);
    return (res);
}

// мэйн нужен был мне , решил оставить, чтобы было понятно что нужно для работы
int main() {
    // str - строка в польской нотации
    char str[30] = "x x x * + -";
    // res = подсчет значения (строка в ПН, x , наш стэк);
    double res = polish_calc(str, 4);
    // вывод результата
    printf("%lf", res);
    return 0;
}