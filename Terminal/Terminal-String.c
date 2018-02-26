#include <Terminal-String.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* str;          // String pointer
    size_t buf_len;     // Total length of the s (with null terminator)
    size_t str_len;     // Length of the string in the buffer
    size_t index;       // Current position in string

} Term_String_Obj_t, *pTerm_String_Obj_t;


pTerm_String_t new_terminal_string() {

    pTerm_String_Obj_t ts = calloc(1,sizeof(Term_String_Obj_t));
    ts->str = malloc(17*sizeof(char));
    ts->buf_len = 17;

    reset_terminal_string((pTerm_String_t) ts);

    return (pTerm_String_t) ts;
}


pTerm_String_t string_to_terminal_string(const char* string) {

    pTerm_String_Obj_t ts = calloc(1,sizeof(Term_String_Obj_t));
    ts->str = strdup(string);
    ts->str_len = strlen(string);
    ts->buf_len = ts->str_len+1;
    ts->index = ts->str_len;

    return (pTerm_String_t) ts;
}


void free_terminal_string(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;
    if (ts->str) {free(ts->str);}
    free(ts);
}


//Debug function
void dump_terminal_string(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    printf("Buffer Length: %zu\n",ts->buf_len);
    printf("String Length: %zu\n",ts->str_len);
    printf("Index: %zu\n",ts->index);
    printf("String: %s\n",ts->str);

    fflush(stdout);

    ts->index = ts->str_len;
}


//Clear all data in the string
void reset_terminal_string(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;
    ts->index = 0;
    ts->str_len = 0;
    ts->str[0] = 0;
}


//Insert a character at the current internal state in the string
void insert_terminal_char(pTerm_String_t t, char c) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    //Do I need to realloc the string??
    if (ts->str_len >= (ts->buf_len - 1)) {
        ts->buf_len+=16;
        ts->str = realloc(ts->str,ts->buf_len * sizeof(char));
    }

    //Move the string
    memmove(ts->str+ts->index+1,ts->str+ts->index,
            ts->str_len - ts->index+1);

    ts->str[ts->index] = c;
    ts->index+=1;
    ts->str_len+=1;

    printf("%c",c);
    printf("%s ",ts->str+ts->index);

    //Now, navigate the cursor back to the index
    size_t temp;
    for (temp = ts->str_len+1; temp > ts->index; --temp) {
        printf("\b");
    }


    fflush(stdout);
}


//Insert a string inside the current position
//  This is used for the tab feature
void insert_terminal_string(pTerm_String_t t, const char* string) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;
    size_t len = strlen(string);

    //Do I need to realloc the string??
    if ((ts->str_len +len) >= (ts->buf_len)) {
        /* Always increase by a multiple of 16 */
        ts->buf_len+=16 * ((len >> 4) + ((len & 0xF) != 0));
        ts->str = realloc(ts->str,ts->buf_len * sizeof(char));
    }

    //Move the string
    memmove(ts->str+ts->index+len,ts->str+ts->index,
            ts->str_len - ts->index+1);

    memcpy(ts->str + ts->index,string,len);
    ts->index+=len;
    ts->str_len+=len;

    printf("%s",string);
    printf("%s ",ts->str+ts->index);

    //Now, navigate the cursor back to the index
    size_t temp;
    for (temp = ts->str_len+1; temp > ts->index; --temp) {
        printf("\b");
    }


    fflush(stdout);
}




//Backspace at the index
void terminal_string_backspace(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    if (ts->index > 0) {

        ts->index-=1;

        //Move the string
        memmove(ts->str+ts->index,ts->str+ts->index+1,
                ts->str_len - ts->index+1);

        ts->str_len-=1;

        //Always add a space to erase the last character
        printf("\b%s ",ts->str+ts->index);

        //Now, navigate the cursor to the index
        size_t temp;
        for (temp = ts->str_len+1; temp > ts->index; --temp) {
            printf("\b");
        }

        fflush(stdout);
    }
}

void terminal_string_delete(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    if (ts->index < ts->str_len) {

        ts->str_len-=1;

         //Move the string
        memmove(ts->str+ts->index,ts->str+ts->index+1,
                ts->str_len - ts->index +1);

        //Print the new string
        printf("%s ",ts->str + ts->index);

        //Now, navigate the cursor to the index
        size_t temp;
        for (temp = ts->str_len+1; temp > ts->index; --temp) {
            printf("\b");
        }

        fflush(stdout);
    }
}




//Move left in the current string
void terminal_string_left(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    if (ts->index > 0) {
        ts->index-=1;
        printf("\b");
        fflush(stdout);
    }
}


//Move right in the current string
void terminal_string_right(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    if (ts->index < ts->str_len) {
        printf("%c",ts->str[ts->index]);
        ts->index+=1;
        fflush(stdout);
    }
}


void terminal_string_start(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    for(; ts->index > 0; --ts->index) {
        printf("\b");
    }
    fflush(stdout);
}

void terminal_string_end(pTerm_String_t t) {
    if (!t) {return;}

    pTerm_String_Obj_t ts = (pTerm_String_Obj_t) t;

    for(; ts->index < ts->str_len; ++ts->index) {
        printf("%c",ts->str[ts->index]);
    }
    fflush(stdout);
}
