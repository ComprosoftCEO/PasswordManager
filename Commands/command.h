//Everything necessary for terminal commands
#ifndef COMMAND_HEADER
#define COMMAND_HEADER

#include <stdint.h>
#include <stddef.h>


//Function definition for the command call
//  argv[0]  = The complete command, including spaces "show all"
//  argv[1+] = Other tokenized arguments "Gmail"
typedef int (*Term_Func_t)(int argc,char** argv);


//Struct we use to store a command (used for tab completion)
//  **Be sure to end the array with COMMAND_END macro
#define COMMAND_END     NULL,NULL,NULL,-1,NULL
typedef struct TERM_COMMAND {
    char* command;
    char* description;
    void* sub_commands;
    int sub_type;
    Term_Func_t toCall;     //NULL = Use my parent
} Term_Command_t, *pTerm_Command_t;



//Sub commands are represented abstractly as:
//  1. A pointer to an array of pTerm_Command_t (SUB_PARAMS macro)
//  2. A function that returns an array of pTerm_Command_t (FUNCTION macro)
//          ***Term_Command_Func_t is the function definition***
//  3. NULL, owns no sub commands
typedef pTerm_Command_t (*Term_Command_Func_t)(void);
#define NO_SUB_PARAMS   NULL,0                      /* Doesn't own sub params */
#define SUB_PARAMS(arr) (pTerm_Command_t) arr,1     /* Owns a command array */
#define FUNCTION(arr)   (void*) arr,2               /* Calls a function that builds a command array */


//List of command structures
extern Term_Command_t MAIN_COMMANDS[];

//Set the command set for using different modes
//  Different modes may use different commands
void set_command_set(pTerm_Command_t set);
pTerm_Command_t get_command_set();


//Used by tab completion
pTerm_Command_t get_accounts(void);
pTerm_Command_t get_commands_list(const char* string);
int do_command(const char* string);


//********Put all commands here:*********



#endif // COMMAND_HEADER Included
