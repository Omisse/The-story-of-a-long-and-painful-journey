#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "input.h"

int main(void) {
    int done = 0;
    while (!done) {
        printf("Enter f(x) here:\n");
        char* string = get_validated_string();
        if (string == NULL) {
            printf("n/a\n");
            done = 1;
        }

        rpn(&string);
        print_grid(string);
        // printf("%s\n", string);
        free(string);
        fflush(stdin);
        fflush(stdout);
        char sym = 32;
        printf("Try again?(y for yes, any other for no))\n");
        scanf("%c", &sym);
        if (sym == 'y' || sym == 'Y') {
            done = 0;
        } else {
            done = 1;
        }
    }
    return 0;
}