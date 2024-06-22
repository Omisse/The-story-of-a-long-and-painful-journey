#define BACKGROUND '.'
#define NODE '*'

#define D_tmin 0
#define D_tmax 80

#define R_tmin 0
#define R_tmax 25


#define D_rmin 0.0
#define D_rmax 4*M_PI

#define R_rmin -1.0
#define R_rmax 1.0

int is_in_range(int val, int min, int max);
double map (double val, double s1, double e1, double s2, double e2);
void print_grid();