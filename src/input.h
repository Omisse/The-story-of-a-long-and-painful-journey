char * get_validated_string();

int validate_input(char **buffer);

int is_in(char val, const char *values);

int check_allowed_chars(char *buff);

int check_brackets(char *buff);

int check_chaining_ops(char *buff);

int check_border_ops(char *buff);

int check_doubles(char *buff);

int isonlydot(char **dotptr, char *startptr);

int check_functions(char *buff);

int is_pattern(char **ptr);

void inv_to_zdiff(char **buff);

void itz_place_bracket(char **buffer, char *from);

char *strrep(char **start, char *to, const char *source);

void rpn(char **buff);

int readtoken(char **from, char **to);

int get_type(char tokenpart);

int eval_op(char op);
