#include <command.h>
#include <tokenizer.h>
#include <string.h>



//Convert the sub command code into a command type
static pTerm_Command_t get_sub_command(pTerm_Command_t cmd) {
    switch(cmd->sub_type) {
        case 1:     /* Pointer type */
            return (pTerm_Command_t) cmd->sub_commands;

        case 2:     /* Function type */
            return ((Term_Command_Func_t) cmd->sub_commands)();

        default:    /* NULL type */
            return NULL;
        }
}


pTerm_Command_t get_commands_list(const char* string) {

    Tokens_t tkn = tokenize_string(string);
    pTerm_Command_t cmd = get_command_set();
    if (!tkn.tokens) {return cmd;}

    size_t i, j;
    for (i = 0; (i < tkn.count) && (cmd != NULL); ++i) {

        //Count the number of matches (for incomplete tab completion tokens)
        size_t index, count = 0;
        for (j = 0; (cmd[j].command != NULL) ; ++j) {
            if (!strncmp(tkn.tokens[i],cmd[j].command,strlen(tkn.tokens[i]))) {++count; index = j;}
        }

        //Find the first match
        if (count == 1) {
            cmd = get_sub_command(cmd+index);
            goto next;
        }

        cmd = NULL;
    next:
        cmd = cmd;  /* I hate having to do this... for the labels */
    }

    free_tokenizer(tkn);
    return cmd;
}
