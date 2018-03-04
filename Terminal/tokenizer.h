#ifndef TOKENIZER_HEADER
#define TOKENIZER_HEADER

#include <stddef.h>

//Struct that holds a list of parsed tokens
typedef struct {
    size_t count;
    char** tokens;
} Tokens_t, *pTokens_t;


//Parse the terminal string
Tokens_t tokenize_string(const char* input);
void free_tokenizer(Tokens_t tkn);



#endif // TOKENIZER_HEADER Included
