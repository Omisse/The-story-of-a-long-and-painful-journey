typedef struct CharStack {
    char** strings;
    int topIndex;
} CharStack;

CharStack chs_init();
int chs_is_empty(CharStack stack);
void chs_push(CharStack* stack, char* value);
char* chs_pop(CharStack* stack);
char *chs_peek(CharStack stack);
void chs_destroy(CharStack *stack);