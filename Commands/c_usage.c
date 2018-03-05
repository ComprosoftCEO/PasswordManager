#include <command.h>
#include <concatenator.h>
#include <stdio.h>
#include <stdlib.h>

int c_usage(size_t argc,const char** argv) {

    pTerm_Command_t cmd = NULL;
    if (!param_test(1,argc-1,PARAM_ATLEAST)) {return CMD_RET_OK;}

    char* str = concatenate_strings(argv+1,argc-1," ");
    if (str != NULL) {cmd = find_command(str);}

    if (cmd == NULL) {

        c_unknown(1,(const char**) &str);

        if (str != NULL) {free(str);}
        return CMD_RET_OK;
    }

    printf("\n%s %s\n\n",str,cmd->params);

    free(str);
    return CMD_RET_OK;
}
