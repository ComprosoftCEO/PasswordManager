#include <command.h>
#include <stdio.h>

int c_unknown(size_t argc,const char** argv) {
    if (argc > 0) {
        printf("%s: Unknown command\n", argv[0]);
    } else {
        printf("%% Unknown Command %%\n");
    }
    return CMD_RET_OK;
}
