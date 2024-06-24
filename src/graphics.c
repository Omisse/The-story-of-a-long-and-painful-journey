#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "polishnot.h"

#define BACKGROUND '.'
#define NODE '*'

#define D_tmin 0
#define D_tmax 80

#define R_tmin 0
#define R_tmax 25

#define D_rmin 0.0
#define D_rmax 4 * M_PI

#define R_rmin -1.0
#define R_rmax 1.0

int is_in_range(int val, int min, int max) { return (val <= max && val >= min); }

double map(double val, double s1, double e1, double s2, double e2) {
    return (s2 + (e2 - s2) * ((val - s1) / (e1 - s1)));
}

void print_grid(char* notation) {
    //printf("print_grid.notation: %s\n", notation);
    int grid[D_tmax][R_tmax];
    for (int y = 0; y < R_tmax; y++) {
        for (int x = 0; x < D_tmax; x++) {
            grid[x][y] = BACKGROUND;
        }
    }
    for (int x = 0; x < D_tmax; x++) {
        double key = map((double)x, (double)D_tmin, (double)(D_tmax - 1), D_rmin,
                         D_rmax);                   // remap x from [0,79] to [0.0,4PI]
        char* notval = malloc((strlen(notation)+1)*sizeof(char));
        notval = strcpy(notval, notation);
        notval = realloc(notval, (1+strlen(notval))*sizeof(char));
        double value = polish_calc(&notval, key);  // here we getting values for mapped x
        free(notval);
        notval = NULL;
        int ypos = (int)round(map(value, R_rmin, R_rmax, (double)R_tmax-1.0,
                                  (double)(R_tmin)));  // remap y from [-1.0, 1.0] to [24,0] (24 is equal to -1 since it will be at the bottom of the grid)
        if (is_in_range(ypos, R_tmin, R_tmax - 1)) {
            grid[x][ypos] = NODE;
        }
        //printf("x: %lf || y: %lf\n", key, value);
        //fflush(stdout);
    }
    for (int y = 0; y < R_tmax; y++) {
        for (int x = 0; x < D_tmax; x++) {
            printf("%c", grid[x][y]);
        }
        printf("\n");
        fflush(stdout);
    }
}
