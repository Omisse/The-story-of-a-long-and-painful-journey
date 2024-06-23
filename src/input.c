#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input.h"
#include "chstack.h"

#define _BUFSIZE 256 //needs some change in scanf(-1) if changed here

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

char* get_validated_string() {
    char* buffer = malloc(sizeof(char)*_BUFSIZE);
    scanf("%255s", buffer);
    if (!validate_input(&buffer)) {
        free(buffer);
        buffer = NULL;
    }
    return buffer;
}

int validate_input(char** buffer) {
    if (*buffer == NULL) {
        return 0;
    }

    int valid = 1;

    remove_spaces(buffer);

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
        if (val == *ptr) { found = 1; }
        ptr++;
    }
    return found;
}

void remove_spaces(char** buff) {
    if (*buff == NULL) {
        return;
    }

    char* new_buffer = malloc(strlen(*buff)+1);
    if (new_buffer == NULL) {
        printf("malloc fail\n");
        free(*buff);
        exit(1);
    }

    char* nbs = new_buffer;
    char* ptr = *buff;
    while (*ptr) {
        if (*ptr != ' ') {
            *new_buffer = *ptr;
            new_buffer++;
        }
        ptr++;
    }
    *new_buffer = '\0';
    free(*buff);
    *buff = nbs;
}

//validation based on pre-existing arrays of possible chars in input without spaces
int check_allowed_chars(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    const char numbers[12] = ".0123456789";
    const char operators[5] = "+-*/";
    const char functions[19] = "sincostanctgsqrtln";
    const char brackets[3] = "()";

    int valid = 1;
    char* ptr = buff;
    while (*ptr && valid) {
        valid = is_in(*ptr, numbers)
                || is_in(*ptr, operators)
                || is_in(*ptr, functions)
                || is_in(*ptr, brackets)
                || *ptr == 'x';
        if (*ptr == 'x' && (is_in(*ptr+1, numbers) || is_in(*ptr-1, numbers))) {
            valid = 0;
        }
        ptr++;
    }
    return valid;
}

//check if '(' amount is equal to ')' amount
//also checks for empty brackets
int check_brackets(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    const char numbers[12] = "x.0123456789";

    char* ptr = buff;
    int bsum = 0;
    int wasnum = 0;
    char lastchar = ' ';
    while (*ptr) {
        if (*ptr == '(') {
            bsum++;
            if (*(ptr+1)=='\0') {
                bsum+=_BUFSIZE;
            }
            wasnum = 0;
        } else if (*ptr == ')') {
            bsum--;
            if (lastchar == ' ' || lastchar == '(') {
                bsum+=_BUFSIZE; //basically making it all invalid;
            }
            if (!wasnum) {
                bsum+=_BUFSIZE;
            }
        }
        if (is_in(*ptr, numbers)) {
            wasnum++;
        }
        if (bsum < 0) {
            bsum+=_BUFSIZE; // to avoid 1+2)*(sin(3) (example) validation
        }
        lastchar = *ptr;
        ptr++;
    }
    return (!bsum);
}

//check if none of operators are exactly after another operator
int check_chaining_ops(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    const char operators[5] = "+-*/";
    char* ptr = buff;
    int valid = 1;
    int is_prev_op = 0;
    while (*ptr && valid) {
        int is_op = is_in(*ptr, operators);
        valid = !(is_op && is_prev_op);
        if (!valid) {
            valid = (*ptr == '-' && (!is_in(*(ptr+1),operators)) && *(ptr+1)!='\0');
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

    const char operators[5] = "+-*/";

    int valid = 0;

    valid = (!(is_in(*buff, operators) && *buff != '-'));
    if (valid) {
        char* ptr = buff;
        while (*ptr) {
            ptr++;
        }
        ptr--;
        valid = (!is_in(*ptr, operators));
    }
    return valid;
}

//checking for double occurences and correct dots placement
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

    //going back
    char* moveptr = (*dotptr)-1;
    int isnum = *moveptr >= '0' && *moveptr <= '9';
    int isonly = isnum;
    while ((moveptr != startptr) && isonly && isnum) {
        isnum = (*moveptr >= '0' && *moveptr <= '9');
        isonly = *moveptr != '.';
        moveptr--;
    }

    if (isonly) {
        //going forth if back is fine
        moveptr = (*dotptr)+1;
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
            *dotptr = moveptr-1;
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
    const char functions[19] = "sincostanctgsqrtln";

    int valid = 1;
    char* ptr = buff;

    while (*ptr && valid) {
        if (is_in(*ptr, functions)) {
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
    const char sin_pattern[5] = "sin(";
    const char cos_pattern[5] = "cos(";
    const char tan_pattern[5] = "tan(";
    const char ctg_pattern[5] = "ctg(";
    const char sqrt_pattern[6] = "sqrt(";
    const char ln_pattern[4] = "ln(";

    int addptr = 0;
    valid = (strstr(*ptr, sin_pattern) != NULL);
    valid = valid || (strstr(*ptr, cos_pattern) != NULL);
    valid = valid || (strstr(*ptr, tan_pattern) != NULL);
    valid = valid || (strstr(*ptr, ctg_pattern) != NULL);
    /*
    avoiding checking same function here
    */
    if (valid) { addptr = 3; }
    else {
        valid = strstr(*ptr, sqrt_pattern) != NULL;
    }
    if (valid && !addptr) {
        addptr = 4;
    }
    valid = valid || (strstr(*ptr, ln_pattern) != NULL);
    if (valid && !addptr) {
        addptr = 2;
    }

    (*ptr) += addptr;

    return valid;
}

#define INVALID 0
#define NUMBER 1
#define OPERATOR 2
#define BRACE 3
#define FUNC 4

void inv_to_zdiff(char** buff) {
    char* ptr = *buff;
    const char operators[5] = "+-*/";
    while (*ptr) {
        if (*ptr == '-') {
            if (ptr == *buff) {
                *buff = strrep(buff, ptr, "(0-");

                itz_place_bracket(buff, strstr(*buff, "(0-")+3);

                ptr = (*buff)-1;
            } else if (is_in((*(ptr-1)), operators) || *(ptr-1) == '(') {
                *buff = strrep(buff, ptr, "(0-");
                /*
                скобки закрываем когда
                a) дойдём до конца буфера
                б) (найдём следующий оператор и кол-во открытых и закрытых скобок будет = 1)
                */
                itz_place_bracket(buff, strstr(*buff, "(0-")+3);

                ptr = (*buff)-1;
            }
        }
        ptr++;
    }
}

void itz_place_bracket(char** buffer,char* from) {
    int bsum = 1;
    int done = 0;
    while (*from && !done) {
        if (*from == '(') {
            bsum++;
        } else if (*from == ')') {
            bsum--;
        } else if (*from == '+' && bsum == 1) {
            *buffer = strrep(buffer, from, ")+");
            done = 1;
        } else if (*from == '-' && bsum == 1) {
            *buffer = strrep(buffer, from, ")-");
            done = 1;
        } else if (*from == '/' && bsum == 1) {
            *buffer = strrep(buffer, from, ")/");
            done = 1;
        } else if (*from == '*' && bsum == 1) {
            *buffer = strrep(buffer, from, ")*");
            done = 1;
        }
        from++;
    }
    if (bsum == 1) {
        *buffer = realloc(*buffer, (strlen(*buffer)+2)*sizeof(char));
        int len = strlen(*buffer);
        *((*buffer)+len) = ')';
        *((*buffer)+len+1) = '\0';
    }
}

char* strrep(char** start, char*to, const char* source) {
    char* new_string = malloc((2+strlen(source)+strlen(*start))*sizeof(char));
    if (new_string == NULL) {
        printf("strins: malloc fail\n");
        free(*start);
        exit(1);
    }
    char* next_string = to+1;
    *to = '\0';
    strcpy(new_string, *start);
    strcat(new_string,source);
    strcat(new_string, next_string);
    free(*start);
    *start = NULL;
    *start = realloc(new_string, 1+strlen(new_string)*sizeof(char));
    if (*start == NULL) {
        printf("strins: malloc fail\n");
        exit(1);
    }
    return *start;
}

char* rpn(char* buff) {
    if (buff == NULL) {
        return 0;
    }

    inv_to_zdiff(&buff);

    CharStack stack = chs_init();
    char* o_str = malloc(sizeof(char)*(strlen(buff)+1));

    if (o_str == NULL) {
        printf("malloc fail\n");
        free(buff);
        exit(1);
    }

    *o_str = '\0';

    char* tmpb = buff;
    while(*tmpb != '\0') {
        char* token = NULL;
        int type = readtoken(&tmpb, &token);
        #ifdef V
        printf("Token: %s|done|\n", token);
        #endif
        if (type == NUMBER) {
            strcat(o_str, token); //check for memleaks please
        } else if (type == FUNC) {
            chs_push(&stack,token);
        } else if (type == BRACE) {
            if (*token == '(') {
                chs_push(&stack, token);
            } else if (*token == ')') {
                char* temp_token = chs_peek(stack);
                while (temp_token != NULL) {
                    temp_token = chs_pop(&stack);
                    if (temp_token != NULL) {
                        if (*temp_token != '(') {
                            strcat(o_str, temp_token); //check for memleaks please
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
            while(temp_token != NULL && eval_op(*temp_token) >= own_prec) {
                temp_token = chs_pop(&stack);
                if (temp_token != NULL) {
                    strcat(o_str, temp_token); //check for memleaks please
                    free(temp_token);
                }
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

    //empty the rest of the stack
    char* token = chs_pop(&stack);
    while (token != NULL) {
        strcat(o_str, token);
        free(token);
        token = chs_pop(&stack);
    }

    chs_destroy(&stack);
    char* spacer;
    while(*spacer) {
        spacer++;
    }
    if (spacer != o_str) {
        spacer--;
        *spacer = '\0';
    }

    free(buff);

    o_str = realloc(o_str, (1+strlen(o_str))*sizeof(char));

    return o_str;
}


/*
#define INVALID 0
#define NUMBER 1
#define OPERATOR 2
#define BRACE 3
#define FUNC 4
*/

int readtoken(char** from, char** to) {
    char* str = *from;
    char* token = malloc(strlen(*from)*sizeof(char));
    int look_for = get_type(*str);
    #ifdef V
    printf("Looking for type: %d\n", look_for);
    #endif
    if (look_for != INVALID) {
        int done = 0;
        while(*str && !done) {
            if (get_type(*str) == look_for){
                token[str-(*from)] = *str;
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
        token[str-(*from)] = ' '; //space at the end of the token
        token[str-(*from)+1] = '\0';
    }

    if (*from != str) {
        *from = str;//jump to next token
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
    const char functions[20] = "sincostanctgsqrtln"; //~ states for unary minus, i know it is wrong
    const char brackets[3] = "()";

    int type = INVALID;
    type = NUMBER*is_in(tokenpart, numbers);
    if (!type) {
        type = OPERATOR*is_in(tokenpart,operators);
    }
    if (!type) {
        type = BRACE*is_in(tokenpart, brackets);
    }
    if(!type) {
        type = FUNC*is_in(tokenpart, functions);
    }
    return type;
}

int eval_op(char op) {
    int eval = 0;
    if (op == '+' || op == '-') {
        eval = 1;
    } else if (op == '/' || op == '*') {
        eval = 2;
    }
    return eval;
}