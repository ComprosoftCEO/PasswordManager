#include <PasswordManager.h>
#include <terminal.h>
#include <command.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct {
    pTerm_Command_t set;
    char* prompt;
} Mode_t;


// **** Add More Modes Here **** //
static Mode_t ALL_MODES[] = {
    {MAIN_COMMANDS,"-> "},
    {EDIT_COMMANDS,"##> "}
};
size_t MODE_COUNT = sizeof(ALL_MODES) / sizeof(ALL_MODES[0]);


void run_terminal() {

    init_terminal();

    int cur_mode = 0;   /* Start in Main Mode */
    set_command_set(ALL_MODES[cur_mode].set);


    while(1) {
        char* buf = readline(ALL_MODES[cur_mode].prompt);
        if (buf) {
            if (strlen(buf) > 0) {add_history(buf);}

            int result = do_command(buf);
            free(buf);

            if (result < 0) {goto done;}
            if (result > 0 && result <= MODE_COUNT) {
                cur_mode = result - 1;
                set_command_set(ALL_MODES[cur_mode].set);
            }
        }
    }

done:
    close_terminal();
    return;
}
