#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "polishnot.h"
#include "input.h"

#define FSTART "sctl"
#define FALL "sincostanctgsqrtln"

#define SIN_S "sin"
#define COS_S "cos"
#define TAN_S "tan"
#define CTG_S "ctg"
#define SQRT_S "sqrt"
#define LN_S "ln"

#define INV_ID -1
#define SIN_ID 0
#define COS_ID 1
#define TAN_ID 2
#define CTG_ID 3
#define SQRT_ID 4
#define LN_ID 5

#define SUM_ID 1
#define DIFF_ID 2
#define MUL_ID 3
#define DIV_ID 4

//#define V

double polish_calc(char **str, double x) {
    #ifdef V
    printf("pcalc.prebuffer: %s\n",str);
    #endif
    xchar_to_num(str, x);
    char* basestr = *str;
    // создание пустого стэка
    stack *stack;
    //инициализация стэка
    stack = init();
    double res;
    #ifdef V
    printf("pcalc.buffer: %s\n",str);
    #endif
    while (**str) {
        #ifdef V
        printf("pcalc.char: %c\n",*str);
        #endif
        if (isnum(**str)) { 
            double val = getnum(str);
            push(stack, val);
            #ifdef V
            printf("pcalc.value: %lf\n", val);
            #endif
            fflush(stdout);
        }
        else if (isfunc(**str)) {
            int func = get_pattern(str);
            #ifdef V
            printf("pcalc.patternID: %d\n", func);
            #endif
            double val = pop(stack);
            if (func == 0) {
                push(stack, sin(val));
            }
            if (func == 1) {
                push(stack, cos(val));
            }
            if (func == 2) {
                push(stack, tan(val));
            }
            if (func == 3) {
                push(stack, 1.0/tan(val));
            }
            if (func == 4) {
                push(stack, sqrt(val));
            }
            if (func == 5) {
                push(stack, log(val));
            }
        } else if (isop(**str)) {
            int op = isop(**str);
            double b = pop(stack);
            double a = pop(stack);
            if (op == SUM_ID) {
                push(stack, a+b);
            }
            if (op == DIFF_ID) {
                push(stack, a-b);
            }
            if (op == MUL_ID) {
                push(stack, a*b);
            }
            if (op == DIV_ID) {
                push(stack, a/b);
            }
            (*str)++;
        } else { (*str)++; }
    }
    res = pop(stack);
    *str = basestr;
    //очистка стека
    destroy(&stack);
    #ifdef V
    printf("pcalc: destroy happened\n");
    #endif
    return (res);
}

int get_pattern(char** from) {
    char* str = malloc((1+strlen(*from))*sizeof(char));
    if (str==NULL) {
        perror("malloc fail\n");
        free(*from);
        exit(1);
    }
    int step = 0;
    while (isfunc(**from)) {
        str[step] = **from;
        (*from)++;
        step++;
    }
    str[step] = '\0';
    str = realloc(str, sizeof(char)*(1+strlen(str)));
    if (str == NULL) {
        perror("malloc fail\n");
        free(*from);
        exit(1);
    }
    #ifdef V
    printf("pcalc.pattern: %s\n", str);
    #endif
    int id = INV_ID;
    if (strcmp(str, SIN_S) == 0) {
        id = SIN_ID;
    } else if (strcmp(str, COS_S) == 0) {
        id = COS_ID;
    } else if (strcmp(str, TAN_S) == 0) {
        id = TAN_ID;
    } else if (strcmp(str, CTG_S) == 0) {
        id = CTG_ID;
    } else if (strcmp(str, SQRT_S) == 0) {
        id = SQRT_ID;
    } else if (strcmp(str, LN_S) == 0) {
        id = LN_ID;
    }
    free(str);
    return id;
}

int isop(char what) {
    int type = 0;
    type = SUM_ID*(what == '+');
    if (!type) { type = DIFF_ID*(what == '-'); }
    if (!type) { type = MUL_ID*(what == '*'); }
    if (!type) { type = DIV_ID*(what == '/'); }
    return type;
}

int isfunc(char what) {
    int func = 0;
    if (strchr(FALL, what) != NULL) {
        func = 1;
    }
    return func;
}

double getnum(char** from) {
    char* tmps = malloc((1+strlen(*from))*sizeof(char));
    if (tmps == NULL) {
        perror("malloc fail\n");
        exit(1);
    }
    int step = 0;
    while (isnum(**from)) {
        tmps[step] = **from;
        (*from)++;
        step++;
    }
    tmps[step] = '\0';
    double result = s_strtod(tmps);
    free(tmps);
    return result;
}

int isnum(char what) {
    int num = 0;
    if ((what >= '0' && what <= '9') || (what == '.')) {
        num = 1;
    }
    return num;
}

void xchar_to_num(char** buff, double x) {
    int done = 0;
    while(!done) {
        char* ptr = strchr(*buff,'x');
        if (ptr == NULL) { done = 1; }
        else {
            char* str = num_to_string(x);
            *buff = strrep(buff, ptr, str);
            free(str);
        }
    }
}

char* s_strrep(char** start, char* to, const char* source) {
    char* new_string = malloc((2 + strlen(source) + strlen(*start)) * sizeof(char));
    if (new_string == NULL) {
        perror("strins: malloc fail\n");
        free(*start);
        exit(1);
    }
    char* next_string = to + 1;
    *to = '\0';
    strcpy(new_string, *start);
    strcat(new_string, source);
    strcat(new_string, next_string);
    free(*start);
    *start = NULL;
    *start = realloc(new_string, (1 + strlen(new_string)) * sizeof(char));
    if (*start == NULL) {
        perror("strins: malloc fail\n");
        exit(1);
    }
    return *start;
}

double s_strtod(char* str_start) {
    char* ptr = malloc((strlen(str_start)+1)*sizeof(char));
    if (ptr == NULL) {
        perror("malloc fail\n");
        free(str_start);
        exit(1);
    }
    ptr = strcpy(ptr, str_start);
    char* ptrs = ptr;
    while((*ptr <= '9' && *ptr >= '0') || *ptr == '.') {
        ptr++;
    }
    *(ptr+1) = '\0';
    double result = atof(ptrs);
    free(ptrs);
    return(result);
}

char* num_to_string(double num) {
    char* str = malloc(sizeof(char)*255);
    if (str == NULL) {
        perror("malloc fail\n");
        exit(1);
    }
    sprintf(str, "%f", num);
    str = realloc(str,(1+strlen(str))*sizeof(char));
    return str;
}

// мэйн нужен был мне , решил оставить, чтобы было понятно что нужно для работы
/*
int main() {
    // str - строка в польской нотации (в обратной польской, кого мы тут обманываем)
    char str[30] = "x x x * + -";
    // res = подсчет значения (строка в ПН, x);
    double res = polish_calc(str, 4);
    // вывод результата
    printf("%lf", res);
    return 0;
}
*/