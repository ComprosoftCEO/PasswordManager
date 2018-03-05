#include <command.h>
#include <concatenator.h>
#include <stdio.h>
#include <stdlib.h>

int c_help(size_t argc, char** argv) {

    pTerm_Command_t cmd = NULL;

    char* str = NULL;
    if (argc <= 1) {cmd = get_command_set();}
    else {
        str = concatenate_strings(argv+1,argc-1," ");
        if (str != NULL) {cmd = get_commands_list(str,0);}
    }

    if (cmd == NULL) {
        if (str == NULL) {
            printf("\nNo help available\n\n");
        } else {
            printf("\nNo help for \"%s\"\n\n",str);
            free(str);
        }

        return CMD_RET_OK;
    }

    printf("\n");
    while ((cmd != NULL) && (cmd->command != NULL)) {
        printf("%-15s\t%s\n",cmd->command,cmd->description);
        cmd++;
    }
    printf("\n");

    free(str);
    return CMD_RET_OK;
}
