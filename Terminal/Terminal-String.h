#ifndef TERMINAL_STRING_HEADER
#define TERMINAL_STRING_HEADER

//Terminal String handles all of the crazy terminal interactions
typedef struct {
    char* str;
} *pTerm_String_t;

pTerm_String_t new_terminal_string();
pTerm_String_t string_to_terminal_string(const char* string);
void free_terminal_string(pTerm_String_t ts);

void dump_terminal_string(pTerm_String_t);
void reset_terminal_string(pTerm_String_t);

void insert_terminal_char(pTerm_String_t, char input);
void insert_terminal_string(pTerm_String_t, const char* input);

//Move around inside the terminal string
void terminal_string_backspace(pTerm_String_t t);
void terminal_string_delete(pTerm_String_t t);
void terminal_string_left(pTerm_String_t t);
void terminal_string_right(pTerm_String_t t);
void terminal_string_start(pTerm_String_t t);
void terminal_string_end(pTerm_String_t t);


#endif // TERMINAL_STRING_HEADER
