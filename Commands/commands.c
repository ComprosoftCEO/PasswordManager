//This is where the entire structure of commands is defined:
#include <command.h>

//Where do we start searching for commands???
static pTerm_Command_t command_set = MAIN_COMMANDS;
void set_command_set(pTerm_Command_t set) {
    if (set != NULL) {command_set = set;}
}
pTerm_Command_t get_command_set() {return command_set;}



Term_Command_t MAIN_COMMANDS[] = {
    {"help","[command]","Display help screen",     c_help, 1, MAIN_COMMANDS, NO_FUNC_PROPAGATE | NO_CMD_FUNCTIONS},
    {"usage","<command>","Show command parameters",c_usage,   1, MAIN_COMMANDS, NO_FUNC_PROPAGATE | NO_CMD_FUNCTIONS},
    {"exit","","Leave the program",                c_exit, NO_SUB_PARAMS},
    {"save","","Write changes to the file",        NULL,   NO_SUB_PARAMS},
    {"reset","","Reset all changes made thus far", NULL,   NO_SUB_PARAMS},
    {"edit","","Enable edit mode", c_edit,  NO_SUB_PARAMS},

    {"show","<type> [account]","Display account information",    c_incomplete, SUB_PARAMS() {
        {"accounts","","List database accounts",  NULL,  NO_SUB_PARAMS},
        {"all","<account>","Show username and password",   NULL,  FUNCTION(get_accounts)},
        {"username","<account>","Show username",           NULL,  FUNCTION(get_accounts)},
        {"password","<account>","Show password",           NULL,  FUNCTION(get_accounts)},
        {COMMAND_END}
    }},

    {COMMAND_END}
};


//Commands only allowed in edit mode
Term_Command_t EDIT_COMMANDS[] = {
    {"help","[command]","Display help screen",              c_help, NO_SUB_PARAMS},
    {"done","","Leave edit mode",          c_done,   NO_SUB_PARAMS},
    {"add","<account>","Add <account> entry to the database",               NULL,     NO_SUB_PARAMS},

    {"update","<type> <account>","Update existing information",   c_incomplete, SUB_PARAMS() {
        {"all",     "<account>","Update username and password for <account>", NULL, FUNCTION(get_accounts)},
        {"username","<account>","Update username only for <account>",         NULL, FUNCTION(get_accounts)},
        {"password","<account>","Update password only for <account>",         NULL, FUNCTION(get_accounts)},
        {COMMAND_END}
    }},

    {"set","<type>","Change master and edit password",  c_incomplete, SUB_PARAMS() {
        {"master-password","","Change the database password", NULL, NO_SUB_PARAMS},
        {"edit-password"  ,"","Change the master password",     NULL, NO_SUB_PARAMS},
        {COMMAND_END}
    }},

    {"show","<type> [account]","Display account information",    c_incomplete, SUB_PARAMS() {
        {"accounts","","List database accounts",  NULL,  NO_SUB_PARAMS},
        {"all","<account>","Show username and password",   NULL,  FUNCTION(get_accounts)},
        {"username","<account>","Show username",           NULL,  FUNCTION(get_accounts)},
        {"password","<account>","Show password",           NULL,  FUNCTION(get_accounts)},
        {COMMAND_END}
    }},

    {COMMAND_END}
};
