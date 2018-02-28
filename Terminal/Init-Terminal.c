#include <Terminal.h>
#include <stdio.h>
#include <readline/readline.h>

int init_terminal() {
    rl_prep_terminal(0);    //This allows us to use rl_read_key
    init_tab_completion();
    return 0;
}

int close_terminal() {
    rl_deprep_terminal();
    return 0;
}
