#include <command.h>
#include <tokenizer.h>
#include <string.h>

pTerm_Command_t find_command(const char* string) {

    Tokens_t tkn = tokenize_string(string);
    pTerm_Command_t pre_cmd = NULL, cmd = get_command_set();
    if (!tkn.tokens) {return NULL;}

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
                pre_cmd = cmd+index;
                cmd = get_sub_command(cmd+index,!(state & NO_CMD_FUNCTIONS));
            } else {
                pre_cmd = cmd+index;
                cmd = NULL;
            }
            goto next;
        }

        cmd = NULL;
    next:
        cmd = cmd;  /* I hate having to do this... for the labels */
    }

    free_tokenizer(tkn);
    return pre_cmd;
}
