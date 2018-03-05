//Nifty little function that returns the sub commands list
#include <command.h>


pTerm_Command_t get_sub_command(pTerm_Command_t cmd, int runFunc) {
    switch(cmd->sub_type) {
        case 1:     /* Pointer type */
            return (pTerm_Command_t) cmd->sub_commands;

        case 2:     /* Function type */
            if (runFunc != 0) {
                return ((Term_Command_Func_t) cmd->sub_commands)();
            }

        default:    /* NULL type */
            return NULL;
        }
}
