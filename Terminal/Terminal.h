#ifndef TERMINAL_HEADER
#define TERMINAL_HEADER

#include <Platform.h>
#include <Keys.h>

#include <stddef.h>
#include <stdint.h>

//Terminal Errors
#define ERROR_STREAM   -1
#define ERROR_CONSOLE  -2


//Struct we use to store a command (used for tab completion)
#define NO_SUB_PARAMS   NULL,0                      /* Doesn't own sub params */
#define SUB_PARAMS(arr) (pTerm_Command_t) arr,1     /* Owns a command array */
#define FUNCTION(arr)   (void*) arr,2               /* Calls a function that builds a command array */
#define COMMAND_END     NULL,NULL,NULL,-1           /* End of the command array list */
typedef struct TERM_COMMAND {
    char* command;
    char* description;
    void* sub_commands;
    int sub_type;
} Term_Command_t, *pTerm_Command_t;

//Struct that holds a list of parsed tokens
typedef struct {
    size_t count;
    char** tokens;
} Term_Tokens_t, *pTerm_Tokens_t;


//List of command structures
extern Term_Command_t MAIN_COMMANDS[];


//Return a negative number on failure
int init_terminal();
void init_tab_completion(); //No need to ever call this function
int close_terminal();


//Parse the terminal string
Term_Tokens_t terminal_tokenizer(const char* input);
void free_tokenizer(Term_Tokens_t tkn);


//Used by tab completion
pTerm_Command_t get_accounts();
pTerm_Command_t get_commands_list(const char* string);

#endif // TERMINAL_HEADER Included
