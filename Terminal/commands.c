#include <Terminal.h>
#include <string.h>

static Term_Command_t SHOW_SUB_COMMANDS[];
static Term_Command_t UPDATE_SUB_COMMANDS[];
static Term_Command_t SET_SUB_COMMANDS[];

Term_Command_t MAIN_COMMANDS[] = {
    {"help","Display help screen",           NO_SUB_PARAMS},
    {"exit","Leave the program",             NO_SUB_PARAMS},
    {"save","Write changes to the file",     NO_SUB_PARAMS},
    {"reset","Reset all changes made thus far", NO_SUB_PARAMS},
    {"save","Write changes to the file",     NO_SUB_PARAMS},
    {"show","Display password information",  SUB_PARAMS(SHOW_SUB_COMMANDS)},
    {"edit","Enable edit mode",              NO_SUB_PARAMS},
    {COMMAND_END}
};


//Commands only allowed in edit mode
Term_Command_t EDIT_COMMANDS[] = {
    {"help","Display help screen",           NO_SUB_PARAMS},
    {"done","Leave edit mode",               NO_SUB_PARAMS},
    {"add","Add account",                    NO_SUB_PARAMS},
    {"update","Update existing information", SUB_PARAMS(UPDATE_SUB_COMMANDS)},
    {"set","Change master and edit password",SUB_PARAMS(SET_SUB_COMMANDS)},
    {"show","Display password information",  SUB_PARAMS(SHOW_SUB_COMMANDS)},
    {COMMAND_END}
};



static Term_Command_t SHOW_SUB_COMMANDS[] = {
    {"accounts","List accounts",    NO_SUB_PARAMS},
    {"all","List accounts",         FUNCTION(get_accounts)},
    {"username","List accounts",    FUNCTION(get_accounts)},
    {"password","List accounts",    FUNCTION(get_accounts)},
    {COMMAND_END}
};


static Term_Command_t UPDATE_SUB_COMMANDS[] = {
    {"all","Update username and password", FUNCTION(get_accounts)},
    {"username","Update username only",    FUNCTION(get_accounts)},
    {"password","Update password only",    FUNCTION(get_accounts)},
    {COMMAND_END}
};

static Term_Command_t SET_SUB_COMMANDS[] = {
    {"master-password","Change the database password", NO_SUB_PARAMS},
    {"edit-password","Change the master password",     NO_SUB_PARAMS},
    {COMMAND_END}
};



//Where do we start searching for commands???
pTerm_Command_t start_com = EDIT_COMMANDS;

pTerm_Command_t get_commands_list(const char* string) {

    Term_Tokens_t tkn = terminal_tokenizer(string);
    pTerm_Command_t cmd = start_com;
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

            //Find the first match:

            switch(cmd[index].sub_type) {
            case 1:
                cmd = (pTerm_Command_t) cmd[index].sub_commands;
                break;

            case 2:
                cmd = ( (pTerm_Command_t(*)(void)) cmd[index].sub_commands)();
                break;

            default:
                cmd = NULL;
                break;
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
