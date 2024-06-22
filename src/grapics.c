#include <math.h>

#include "graphics.h"

Vec2i real_to_term(Vec2 rposition) {
    Vec2i position;
    position.x = (int)floor(map(rposition.x, D_rmin, D_rmax, (double)D_tmin, (double)D_tmax));
    position.y = (int)floor(map(rposition.y, R_rmin, R_rmax, (double)R_tmin, (double)R_tmax));
    return position;
}

Vec2 term_to_real(Vec2i tposition) {
    Vec2 position;
    tposition.x = map(position.x, (double)D_tmin, (double)D_tmax, D_rmin, D_rmax);
    tposition.y = map(position.y, (double)R_tmin, (double)R_tmax, R_rmin, R_rmax);
    return position;
}

int is_in_rangef(double val, double min, double max) {
    return (val <= max && val >= min);
}

int is_in_range(int val, int min, int max) {
    return (val <= max && val >= min);
}

double map (double val, double s1, double e1, double s2, double e2) {
    return (s2 + (e2 - s2) * ((val - s1) / (e1 - s1)));
}

