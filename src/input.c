#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chstack.h"

#define _BUFSIZE 255  // needs some change in scanf(-1) if changed here
//#define V

#define OPS "+-/*"
#define NUM_DOT ".0123456789"
#define NUM_X "x.0123456789"
#define FLETTERS "sincostanctgsqrtln"
#define BRACKETS "()"

#define SIN_S "sin("
#define COS_S "cos("
#define TAN_S "tan("
#define CTG_S "ctg("
#define SQRT_S "sqrt("
#define LN_S "ln("

/*
int main() {
    char* string = get_validated_string();
    #ifdef V
    if (string == NULL) {
        printf("Invalid string\n");
    } else {
        printf("Valid string\n");
        printf("%s\n", string);
    }
    #endif
    string = rpn(string);
    printf("%s\n", string);
    free(string);
}
*/

char* get_validated_string() {
    char* buffer = malloc(sizeof(char) * _BUFSIZE);
    int step = 0;
    char ch = ' ';
    while (step < _BUFSIZE-1 && ch != '\0') {
        scanf("%c", &ch);
        if (ch == '\n') { ch = '\0'; }
        if (ch != ' ') {
            buffer[step] = ch;
            step++;
        }
    }
    fflush(stdin);
    if (!validate_input(&buffer)) {
        free(buffer);
        buffer = NULL;
    }
    return buffer;
}

#pragma region validation

int validate_input(char** buffer) {
    if (*buffer == NULL) {
        return 0;
    }

    int valid = 1;
    #ifdef V
    printf("validation.buffer: %s\n",*buffer);
    #endif
    valid = check_allowed_chars(*buffer);
    if (valid) {
#ifdef V
        printf("chars good\n");
#endif
        valid = check_brackets(*buffer);
    }
    if (valid) {
#ifdef V
        printf("brackets good\n");
#endif
        valid = check_chaining_ops(*buffer);
    }
    if (valid) {
#ifdef V
        printf("chaining good\n");
#endif
        valid = check_border_ops(*buffer);
    }
    if (valid) {
#ifdef V
        printf("borders good\n");
#endif
        valid = check_doubles(*buffer);
    }
    if (valid) {
#ifdef V
        printf("doubles good\n");
#endif
        valid = check_functions(*buffer);
    }
    if (valid) {
#ifdef V
        printf("functions good\n");
#endif
    }

    return valid;
}

int is_in(char val, const char* values) {
    if (values == NULL) {
        return 0;
    }

    int found = 0;
    const char* ptr = values;
    while (*ptr && !found) {
        if (val == *ptr) {
            found = 1;
        }
        ptr++;
    }
    return found;
}

// validation based on pre-existing arrays of possible chars in input without spaces
int check_allowed_chars(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    int valid = 1;
    char* ptr = buff;
    while (*ptr && valid) {
        valid = is_in(*ptr, NUM_DOT) || is_in(*ptr, OPS) || is_in(*ptr, FLETTERS) || is_in(*ptr, BRACKETS) ||
                *ptr == 'x';
        if (*ptr == 'x' && (is_in(*ptr + 1, NUM_DOT) || is_in(*ptr - 1, NUM_DOT))) {
            valid = 0;
        }
        #ifdef V
        printf("ptr: %c ||", *ptr);
        printf("valid: %d\n", valid);
        #endif
        ptr++;
    }
    return valid;
}

// check if '(' amount is equal to ')' amount
// also checks for empty brackets
int check_brackets(char* buff) {
    if (buff == NULL) {
        return 0;
    }
    char* ptr = buff;
    int bsum = 0;
    int wasnum = 0;
    char lastchar = ' ';
    while (*ptr) {
        if (*ptr == '(') {
            bsum++;
            if (*(ptr + 1) == '\0') {
                bsum += _BUFSIZE;
            }
            wasnum = 0;
        } else if (*ptr == ')') {
            bsum--;
            if (lastchar == ' ' || lastchar == '(') {
                bsum += _BUFSIZE;  // basically making it all invalid;
            }
            if (!wasnum) {
                bsum += _BUFSIZE;
            }
        }
        if (is_in(*ptr, NUM_X)) {
            wasnum++;
        }
        if (bsum < 0) {
            bsum += _BUFSIZE;  // to avoid 1+2)*(sin(3) (example) validation
        }
        lastchar = *ptr;
        ptr++;
    }
    return (!bsum);
}

// check if none of operators are exactly after another operator
int check_chaining_ops(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    char* ptr = buff;
    int valid = 1;
    int is_prev_op = 0;
    while (*ptr && valid) {
        int is_op = is_in(*ptr, OPS);
        valid = !(is_op && is_prev_op);
        if (!valid) {
            valid = (*ptr == '-' && (!is_in(*(ptr + 1), OPS)) && *(ptr + 1) != '\0');
        }
        is_prev_op = is_op;
        ptr++;
    }
    return valid;
}

int check_border_ops(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    int valid = 0;

    valid = (!(is_in(*buff, OPS) && *buff != '-'));
    if (valid) {
        char* ptr = buff;
        while (*ptr) {
            ptr++;
        }
        ptr--;
        valid = (!is_in(*ptr, OPS));
    }
    return valid;
}

// checking for double occurences and correct dots placement
int check_doubles(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    int valid = *buff != '.';
    if (valid) {
        char* ptr = buff;
        /*
        could jump by strchr actually, but im a little scared of it
        mostly because of my stupidity
        */
        while (*ptr && valid) {
            if (*ptr == '.') {
                valid = isonlydot(&ptr, buff);
            }
            ptr++;
        }
    }
    return valid;
}

/*
marching back and forth to ensure that there is only numbers b4
(until other non-number and non-dot)
*/
int isonlydot(char** dotptr, char* startptr) {
    if (*dotptr == NULL || startptr == NULL) {
        return 0;
    }

    // going back
    char* moveptr = (*dotptr) - 1;
    int isnum = *moveptr >= '0' && *moveptr <= '9';
    int isonly = isnum;
    while ((moveptr != startptr) && isonly && isnum) {
        isnum = (*moveptr >= '0' && *moveptr <= '9');
        isonly = *moveptr != '.';
        moveptr--;
    }

    if (isonly) {
        // going forth if back is fine
        moveptr = (*dotptr) + 1;
        isnum = *moveptr >= '0' && *moveptr <= '9';
        isonly = isnum;
        while (*moveptr && isonly && isnum) {
            isnum = *moveptr >= '0' && *moveptr <= '9';
            isonly = *moveptr != '.';
            moveptr++;
        }
        /*
        redirecting search to field of uncertainty if all fine
        before: 123.456 *dotptr = address of [3] id,
        after: 123.456 *dotptr = address of [6] id.
        */
        if (isonly) {
            *dotptr = moveptr - 1;
        }
    }
    return isonly;
}
/*
iterating (again) through buffer in order to find function patterns
unrecognized pattern will be considered invalid
*/
int check_functions(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    int valid = 1;
    char* ptr = buff;

    while (*ptr && valid) {
        if (is_in(*ptr, FLETTERS)) {
            valid = is_pattern(&ptr);
        }
        ptr++;
    }

    return valid;
}

int is_pattern(char** ptr) {
    if (*ptr == NULL) {
        return 0;
    }
    int valid = 0;
    int addptr = 0;
    valid = (strstr(*ptr, SIN_S) != NULL);
    valid = valid || (strstr(*ptr, COS_S) != NULL);
    valid = valid || (strstr(*ptr, TAN_S) != NULL);
    valid = valid || (strstr(*ptr, CTG_S) != NULL);
    /*
    avoiding checking same function here
    */
    if (valid) {
        addptr = 3;
    } else {
        valid = strstr(*ptr, SQRT_S) != NULL;
    }
    if (valid && !addptr) {
        addptr = 4;
    }
    valid = valid || (strstr(*ptr, SQRT_S) != NULL);
    if (valid && !addptr) {
        addptr = 2;
    }

    (*ptr) += addptr;

    return valid;
}
#pragma endregion

#define INVALID 0
#define NUMBER 1
#define OPERATOR 2
#define BRACE 3
#define FUNC 4

void inv_to_zdiff(char** buff) {
    char* ptr = *buff;
    while (*ptr) {
        if (*ptr == '-') {
            if (ptr == *buff) {
                *buff = strrep(buff, ptr, "(0-");

                itz_place_bracket(buff, strstr(*buff, "(0-") + 3);

                ptr = (*buff) - 1;
            } else if (is_in((*(ptr - 1)), OPS) || *(ptr - 1) == '(') {
                *buff = strrep(buff, ptr, "(0-");
                /*
                скобки закрываем когда
                a) дойдём до конца буфера
                б) (найдём следующий оператор и кол-во открытых и закрытых скобок будет = 1)
                */
                itz_place_bracket(buff, strstr(*buff, "(0-") + 3);

                ptr = (*buff) - 1;
            }
        }
        ptr++;
    }
}

void itz_place_bracket(char** buffer, char* from) {
    int bsum = 1;
    int done = 0;
    while (*from && !done) {
        if (*from == '(') {
            bsum++;
        } else if (*from == ')') {
            bsum--;
        } else if (*from == '+' && bsum == 1) {
            *buffer = strrep(buffer, from, ")+");
            bsum = 0;
            done = 1;
        } else if (*from == '-' && bsum == 1) {
            *buffer = strrep(buffer, from, ")-");
            bsum = 0;
            done = 1;
        } else if (*from == '/' && bsum == 1) {
            *buffer = strrep(buffer, from, ")/");
            bsum = 0;
            done = 1;
        } else if (*from == '*' && bsum == 1) {
            *buffer = strrep(buffer, from, ")*");
            done = 1;
            bsum = 0;
        }
        from++;
    }
    if (bsum == 1) {
        *buffer = realloc(*buffer, (strlen(*buffer) + 2) * sizeof(char));
        int len = strlen(*buffer);
        *((*buffer) + len) = ')';
        *((*buffer) + len + 1) = '\0';
    }
}

char* strrep(char** start, char* to, const char* source) {
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
#pragma region reverse polish
void rpn(char** buffer) {
    char* buff = *buffer;
    if (buff == NULL) {
        return;
    }
    inv_to_zdiff(buffer);
    buff = *buffer;

    CharStack stack = chs_init();
    char* o_str = (char*)malloc(sizeof(char) * (strlen(buff)*2));

    if (o_str == NULL) {
        perror("malloc fail\n");
        free(buff);
        exit(1);
    }

    *o_str = '\0';

    char* tmpb = buff;
    while (*tmpb != '\0') {
        char* token = NULL;
        int type = readtoken(&tmpb, &token);
#ifdef V
        printf("Token: %s|done|\n", token);
#endif
        if (type == NUMBER) {
            strcat(o_str, token);  // check for memleaks please
        } else if (type == FUNC) {
            chs_push(&stack, token);
        } else if (type == BRACE) {
            if (*token == '(') {
                chs_push(&stack, token);
            } else if (*token == ')') {
                char* temp_token = chs_peek(stack);
                while (temp_token != NULL) {
                    temp_token = chs_pop(&stack);
                    if (temp_token != NULL) {
                        if (*temp_token != '(') {
                            strcat(o_str, temp_token);  // check for memleaks please
                        }
                        free(temp_token);
                        temp_token = chs_peek(stack);
                    }
                }
            }
        } else if (type == OPERATOR) {
            /*
            operator priority
            "/,*"
            "+,-"
            */ //1*2/3-4*5*-6-x
            int own_prec = eval_op(*token);
            char* temp_token = chs_peek(stack);
            while (temp_token != NULL && eval_op(*temp_token) >= own_prec) {
                temp_token = chs_pop(&stack);
                if (temp_token != NULL) {
                    strcat(o_str, temp_token);  // check for memleaks please
                    free(temp_token);
                    temp_token = NULL;
                }
                temp_token = chs_peek(stack);
            }
            chs_push(&stack, token);
        }
        if (token != NULL) {
            free(token);
            token = NULL;
        }
#ifdef V
        printf("Out: %s\n", o_str);
#endif
    }
    // empty the rest of the stack
    char* token = chs_pop(&stack);
    while (token != NULL) {
        strcat(o_str, token);
        free(token);
        token = chs_pop(&stack);
    }

    chs_destroy(&stack);
#ifdef V
    printf("rpn.prenotation: %s\n", o_str);
#endif
    free(*buffer);
    *buffer = (char*)realloc(o_str, (2+strlen(o_str))*sizeof(char));
#ifdef V
    printf("rpn.notation:\n%s\n", *buffer);
    fflush(stdout);
#endif
}

#pragma region tokens
/*
#define INVALID 0
#define NUMBER 1
#define OPERATOR 2
#define BRACE 3
#define FUNC 4
*/

int readtoken(char** from, char** to) {
    char* str = *from;
    char* token = malloc(strlen(*from) * sizeof(char));
    int look_for = get_type(*str);
#ifdef V
    printf("Looking for type: %d\n", look_for);
#endif
    if (look_for != INVALID) {
        int done = 0;
        while (*str && !done) {
            if (get_type(*str) == look_for) {
                token[str - (*from)] = *str;
                if (look_for == BRACE) {
                    done = 1;
                }
                str++;
            } else {
                done = 1;
            }
        }
    }

    if (look_for == INVALID) {
        free(token);
        token = NULL;
    } else {
        token[str - (*from)] = ' ';  // space at the end of the token
        token[str - (*from) + 1] = '\0';
    }

    if (*from != str) {
        *from = str;  // jump to next token
    }
#ifdef V
    printf("Next token starts from: %s\n", *from);
#endif
    *to = token;
    return look_for;
}

/*
#define INVALID 0
#define NUMBER 1
#define OPERATOR 2
#define BRACE 3
#define FUNC 4
*/

int get_type(char tokenpart) {
    const char numbers[13] = ".x0123456789";
    const char operators[5] = "+-*/";
    const char functions[20] = "sincostanctgsqrtln";  //~ states for unary minus, i know it is wrong
    const char brackets[3] = "()";

    int type = INVALID;
    type = NUMBER * is_in(tokenpart, numbers);
    if (!type) {
        type = OPERATOR * is_in(tokenpart, operators);
    }
    if (!type) {
        type = BRACE * is_in(tokenpart, brackets);
    }
    if (!type) {
        type = FUNC * is_in(tokenpart, functions);
    }
    return type;
}
#pragma endregion

int eval_op(char op) {
    int eval = 0;
    if (op == '+' || op == '-') {
        eval = 1;
    } else if (op == '/' || op == '*') {
        eval = 2;
    }
    return eval;
}
#pragma endregion