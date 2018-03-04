#include <command.h>
#include <tokenizer.h>
#include <string.h>



//Where do we start searching for commands???
pTerm_Command_t start_com = MAIN_COMMANDS;


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
    pTerm_Command_t cmd = start_com;
    if (!tkn.tokens) {return cmd;}

    size_t i, j;
    for (i = 0; (i < tkn.count) && (cmd != NULL); ++i) {
        for (j = 0; (cmd[j].command != NULL) ; ++j) {

            //Find the first match:
            if (!strcmp(tkn.tokens[i],cmd[j].command)) {

                cmd = get_sub_command(cmd+j);
                goto next;
            }
        }

        cmd = NULL;
    next:
        cmd = cmd;  /* I hate having to do this... for the labels */
    }

    free_tokenizer(tkn);
    return cmd;
}
