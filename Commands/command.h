//Everything necessary for terminal commands
#ifndef COMMAND_HEADER
#define COMMAND_HEADER

#include <stdint.h>
#include <stddef.h>
#include <dynamic-array.h>


//Function definition for the command call
//  argv[0]  = The complete command, including spaces "show all"
//  argv[1+] = Other tokenized arguments "Gmail"
//
//  -Numb = Exit
//  0     = Okay
//  1+    = Switch mode
typedef int (*Term_Func_t)(size_t argc,const char** argv);

//Various returns:
#define CMD_RET_EXIT         -1
#define CMD_RET_OK            0
#define CMD_RET_MAIN_MODE     1    /* Switch to main mode */
#define CMD_RET_EDIT_MODE     2    /* Switch to edit mode */




//Struct we use to store a command (used for tab completion)
//  **Be sure to end the array with COMMAND_END macro
#define COMMAND_END     NULL,NULL,NULL,NULL,-1,NULL
#define NO_FUNC_PROPAGATE  0x1  /* Do not propagate function searching */
#define NO_CMD_SEARCHING   0x2  /* Do not propagate command searching */
#define NO_CMD_FUNCTIONS   0x4  /* Do not propagate calling command functions */
typedef struct TERM_COMMAND {
    char* command;
    char* params;           // List of parameters
    char* description;
    Term_Func_t toCall;     // NULL = Use my parent
    int sub_type;
    void* sub_commands;
    int propagate;
} Term_Command_t, *pTerm_Command_t;


//Sub commands are represented abstractly as:
//  1. A pointer to an array of pTerm_Command_t (SUB_PARAMS macro)
//  2. A function that returns an array of pTerm_Command_t (FUNCTION macro)
//          ***Term_Command_Func_t is the function definition***
//  3. NULL, owns no sub commands
pTerm_Command_t get_sub_command(pTerm_Command_t cmd, int runFunc);
typedef pTerm_Command_t (*Term_Command_Func_t)(void);
#define NO_SUB_PARAMS   0,NULL                      /* Doesn't own sub params */
#define SUB_PARAMS()    1,(void*)(Term_Command_t[])        /* Owns a command array */
#define FUNCTION(func)  2,((void*) func)              /* Calls a function that builds a command array */


//List of command structures
extern Term_Command_t MAIN_COMMANDS[];
extern Term_Command_t EDIT_COMMANDS[];


//Set the command set for using different modes
//  Different modes may use different commands
void set_command_set(pTerm_Command_t set);
pTerm_Command_t get_command_set();


//Used by tab completion
pTerm_Command_t get_accounts(void);
pTerm_Command_t get_commands_list(const char* string,int runFunc);
pTerm_Command_t find_command(const char* string);

//Used when running a command
int do_command(const char* string);

//Convenient parameter error message
//  atLeast - If 0, expecting exactly equal
//            If 1, expecting equal or greater than
//
//  Returns 1 on success, 0 on failure
#define PARAM_EXACT   0
#define PARAM_ATLEAST 1
int param_test(size_t expected, size_t given, int atLeast);


//********Put all commands here:*********
int c_nothing(size_t argc,const char** argv);
int c_incomplete(size_t argc,const char** argv);
int c_unknown(size_t argc,const char** argv);
int c_help(size_t argc,const char** argv);
int c_usage(size_t argc,const char** argv);
int c_exit(size_t argc,const char** argv);
int c_edit(size_t argc,const char** argv);
int c_done(size_t argc,const char** argv);


#endif // COMMAND_HEADER Included
