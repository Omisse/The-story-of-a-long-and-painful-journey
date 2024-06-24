#ifndef POLISH
#define POLISH

double polish_calc(char **str, double x);

int get_pattern(char **from);

int isop(char what);

int isfunc(char what);

double getnum(char **from);

int isnum(char what);

void xchar_to_num(char **buff, double x);

char* s_strrep(char** start, char* to, const char* source);

double s_strtod(char *str_start);

char *num_to_string(double num);

#endif