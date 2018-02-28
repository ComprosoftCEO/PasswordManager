#include <Terminal.h>
#include <dynamic-array.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NORMAL,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} e_STATE;


//Return an array of strings
//
//  Escape Character: (\)
//  Quote Characters: ' and "
Term_Tokens_t terminal_tokenizer(const char* input) {

    Term_Tokens_t tkn = {0,0};
    if (!input) {return tkn;}

    size_t index = 0;
    char* toAdd, c;
    e_STATE state = NORMAL;
    int isRaw = 0;
    pDynamic_Arr_t str_arr = new_dynamic_array(sizeof(char));
    pDynamic_Arr_t tokens = new_dynamic_array(sizeof(char*));

    //Start the parsing
    while (input[index] != 0) {

        c = input[index];
        ++index;

        //Force raw data
        if (isRaw != 0) {
            add_array_element(str_arr,&c);
            isRaw = 0;
            continue;
        }


        if (c == ' ') {
            if (state != NORMAL) {  /* We are in quotes */
                add_array_element(str_arr,&c);
                continue;
            }

            if (str_arr->ptr == NULL) {continue; /* Don't flush multiple spaces */}

            c = 0;  /* Add NULL-Terminator */
            add_array_element(str_arr,&c);
            toAdd = flush_dynamic_array(str_arr);
            add_array_element(tokens,&toAdd);
        }

        //Backslash escape
        else if (c == '\\') {
            isRaw = 1;
            continue;
        }

        //Escape with double quotes
        else if (c == '"') {
            if (state == NORMAL) {state = DOUBLE_QUOTE;}
            else if (state == DOUBLE_QUOTE) {state = NORMAL;}
            else {add_array_element(str_arr,&c);}
        }

        //Escape with single quotes
        else if (c == '\'') {
            if (state == NORMAL) {state = SINGLE_QUOTE;}
            else if (state == SINGLE_QUOTE) {state = NORMAL;}
            else {add_array_element(str_arr,&c);}
        }

        //Other characters
        else {
            add_array_element(str_arr,&c);
        }
    }

    //Do one final flush (ignore NULL values)
    if (str_arr->ptr) {
        c = 0; /* Add NULL-Terminator */
        add_array_element(str_arr,&c);
        toAdd = flush_dynamic_array(str_arr);
        add_array_element(tokens,&toAdd);
    }
    free_dynamic_array(str_arr);

    tkn.count = get_array_count(tokens);
    tkn.tokens = (char**) flush_dynamic_array(tokens);
    free_dynamic_array(tokens);

    return tkn;
}



void free_tokenizer(Term_Tokens_t tkn) {
    if (tkn.tokens == NULL) {return;}

    size_t i = 0;
    for (i = 0; i < tkn.count; ++i) {
        free(tkn.tokens[i]);
    }
    free(tkn.tokens);
}
