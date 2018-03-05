//Get the list of account
//  TODO: Make this actually do something...
#include <PasswordManager.h>
#include <terminal.h>
#include <command.h>

pTerm_Command_t get_accounts() {

    static Term_Command_t COMS[] = {
        {"Twitter"},
        {"Gmail"},
        {"Facebook"},
        {"FactBook"},
        {"Instagram"},
        {"Tumbler"},
        {COMMAND_END}
    };

    return COMS;
}
