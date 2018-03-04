//Get the list of account
//  TODO: Make this actually do something...
#include <PasswordManager.h>
#include <terminal.h>
#include <command.h>

pTerm_Command_t get_accounts() {

    static Term_Command_t COMS[] = {
        {"Gmail",NULL,NO_SUB_PARAMS},
        {"Facebook",NULL,NO_SUB_PARAMS},
        {"Instagram",NULL,NO_SUB_PARAMS},
        {"Twitter",NULL,NO_SUB_PARAMS},
        {"Tumbler",NULL,NO_SUB_PARAMS}
    };

    return COMS;
}
