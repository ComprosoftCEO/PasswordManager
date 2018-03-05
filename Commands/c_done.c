#include <command.h>

//Switch back to main mode
int c_done(size_t argc,const char** argv) {
    if (!param_test(0,argc-1,PARAM_EXACT)) {return CMD_RET_OK;}
    return CMD_RET_MAIN_MODE;
}
