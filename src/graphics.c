#include "graphics.h"
//#include "pnotation.h"

#include <math.h>
#include <stdio.h>

int is_in_range(int val, int min, int max) { return (val <= max && val >= min); }

double map(double val, double s1, double e1, double s2, double e2) {
    return (s2 + (e2 - s2) * ((val - s1) / (e1 - s1)));
}

// void print_grid(char* notation) {
void print_grid() {
    int grid[D_tmax][R_tmax];
    for (int y = 0; y < R_tmax; y++) {
        for (int x = 0; x < D_tmax; x++) {
            grid[x][y] = BACKGROUND;
        }
    }
    for (int x = 0; x < D_tmax; x++) {
        double key = map((double)x, (double)D_tmin, (double)(D_tmax - 1), D_rmin,
                         D_rmax);  // remap x from [0,79] to [0.0,4PI]
        double value = cos(key);   // here we getting values for mapped x
        //double value = calc(notation, key)
        int ypos = (int)round(map(value, R_rmin, R_rmax, (double)R_tmin,
                                  (double)(R_tmax - 1)));  // remap y from [-1.0, 1.0] to [0,25]
        if (is_in_range(ypos, R_tmin, R_tmax - 1)) {
            grid[x][ypos] = NODE;
        }
    }
    for (int y = 0; y < R_tmax; y++) {
        for (int x = 0; x < D_tmax; x++) {
            printf("%c", grid[x][y]);
        }
        printf("\n");
    }
}
