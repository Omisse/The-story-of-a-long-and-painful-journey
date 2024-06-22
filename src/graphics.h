#define D_tmin 0
#define D_tmax 80

#define R_tmin 0
#define R_tmax 25


#define D_rmin 0.0
#define D_rmax 4*M_PI

#define R_rmin -1.0
#define R_rmax 1.0

typedef struct {
    double x;
    double y;
} Vec2;

typedef struct {
    int x;
    int y;
} Vec2i;

Vec2 term_to_real(Vec2i tposition);
Vec2i real_to_term(Vec2 rposition);
int is_in_rangef(double val, double min, double max);
int is_in_range(int val, int min, int max);
double map (double val, double s1, double e1, double s2, double e2);

double map (double val, double s1, double e1, double s2, double e2) {
    return (s2 + (e2 - s2) * ((val - s1) / (e1 - s1)));
}