#include <command.h>

//Enable edit mode
int c_edit(size_t argc, char** argv) {
    if (!param_test(0,argc-1,PARAM_EXACT)) {return CMD_RET_OK;}
    return CMD_RET_EDIT_MODE;
}
