#include <command.h>
#include <stdio.h>

int c_incomplete(size_t argc,const char** argv) {
    printf("%% Incomplete Command %%\n");
    return CMD_RET_OK;
}
