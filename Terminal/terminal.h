#ifndef TERMINAL_HEADER
#define TERMINAL_HEADER

#include <platform.h>
#include <stddef.h>
#include <stdint.h>






//Return a negative number on failure
int init_terminal();
void init_tab_completion(); //No need to ever call this function
int close_terminal();






#endif // TERMINAL_HEADER Included
