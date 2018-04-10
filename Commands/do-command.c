#include <command.h>
#include <tokenizer.h>
#include <dynamic-array.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//Returns NULL on failure
//  Do not free the dynamic array if NULL is returned
//  Be sure to call free_dynamic_array with free to avoid strdup memory leak
static Term_Func_t get_command_function(const char* string, pDynamic_Arr_t* arr) {

    Tokens_t tkn = tokenize_string(string);
    pDynamic_Arr_t cmd_token = new_dynamic_array(sizeof(char));
    (*arr) = new_dynamic_array(sizeof(char*));
    pTerm_Command_t pre_cmd = NULL, cmd = get_command_set();
    if (!tkn.tokens) {return c_nothing; /* No Input = Do Nothing */}

    size_t i, j;
    int state = 0;
    for (i = 0; (i < tkn.count) && (cmd != NULL) && (cmd->toCall != NULL); ++i) {

        //Count the number of matches (for incomplete tab completion tokens)
        size_t index, count = 0;
        for (j = 0; (cmd[j].command != NULL) ; ++j) {
            if (!strncmp(tkn.tokens[i],cmd[j].command,strlen(tkn.tokens[i]))) {++count; index = j;}
        }

        //Find the first match
        if (count == 1) {

            if (i > 0) {    //Add space between commands, but not on front
                char temp = ' ';
                add_array_element(cmd_token,&temp);
            }
            add_array_elements(cmd_token,cmd[index].command,strlen(cmd[index].command));

            pre_cmd = cmd+index;
            state |= cmd[index].propagate;

            if ((state & NO_FUNC_PROPAGATE) == 0) {
                cmd = get_sub_command(cmd+index,0);
            } else {
                cmd = NULL;
            }
            goto next;
        }

        cmd = NULL;
    next:
        cmd = cmd;  /* I hate having to do this... for the labels */
    }

    //Copy tokens
    if (pre_cmd == NULL) {
        free_dynamic_array(*arr,NULL);
        free_dynamic_array(cmd_token,NULL);
        free_tokenizer(tkn);
        return NULL;
    }

    char* str = flush_dynamic_array(cmd_token);
    add_array_element(*arr,&str);
    for (; i < tkn.count; ++i) {
        str = strdup(tkn.tokens[i]);
        add_array_element(*arr,&str);
    }

    free_dynamic_array(cmd_token,NULL);
    free_tokenizer(tkn);

    return pre_cmd->toCall;
}



int do_command(const char* string) {

    pDynamic_Arr_t arr;
    Term_Func_t f = get_command_function(string,&arr);
    if (strlen(string) <= 0) {return CMD_RET_OK;}

    if (f != NULL) {

        int result = f(get_array_count(arr),(const char**) arr->ptr);
        free_dynamic_array(arr,free);
        return result;
    }

    //Called if the command is unknown
    c_unknown(1,&string);
    return CMD_RET_OK;
}
