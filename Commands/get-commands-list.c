#include <command.h>
#include <tokenizer.h>
#include <string.h>


pTerm_Command_t get_commands_list(const char* string, int runFunc) {

    Tokens_t tkn = tokenize_string(string);
    pTerm_Command_t cmd = get_command_set();
    if (!tkn.tokens) {return cmd;}

    size_t i, j;
    int state = 0;
    for (i = 0; (i < tkn.count) && (cmd != NULL); ++i) {

        //Count the number of matches (for incomplete tab completion tokens)
        size_t index, count = 0;
        for (j = 0; (cmd[j].command != NULL) ; ++j) {
            if (!strncmp(tkn.tokens[i],cmd[j].command,strlen(tkn.tokens[i]))) {++count; index = j;}
        }

        //Find the first match
        if (count == 1) {
            state |= cmd[index].propagate;
            if ((state & NO_CMD_SEARCHING) == 0) {
                cmd = get_sub_command(cmd+index,runFunc && !(state & NO_CMD_FUNCTIONS));
            } else {
                cmd = NULL;
            }
            goto next;
        }

        cmd = NULL;
    next:
        cmd = cmd;  /* I hate having to do this... for the labels */
    }

    free_tokenizer(tkn);
    return cmd;
}
