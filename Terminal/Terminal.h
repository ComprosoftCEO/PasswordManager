#ifndef TERMINAL_HEADER
#define TERMINAL_HEADER

#include <Platform.h>
#include <Keys.h>
#include <Terminal-String.h>

#include <stdint.h>

//Terminal Errors
#define ERROR_STREAM   -1
#define ERROR_CONSOLE  -2


//Return a negative number on failure
int init_terminal();
int close_terminal();

int terminal_getch();







#endif // TERMINAL_HEADER Included
