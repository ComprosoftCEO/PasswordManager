#include <PasswordManager.h>

const char* arr[] = {
    "Hello World! This string is just tooooooooooo long to store in one chunk!",
    "Here is another string for storage.",
    "598oja975mv -rrg'vfjvjm4urvamflfkqjfvnbr fa ;fq'e[}{}{FJV Fj",
    "Tiny string...",
    "BIGGGGGGGGGGGGGGG STring",
    "Yet another sting for testing purposes in this code.",
    "abcdefghijklmnopqrstuvwxyz",
    "121"
};

void (*TO_DO[])(pTerm_String_t) = {
/* Unknown   */    NULL,
/* Up        */    NULL,
/* Down      */    NULL,
/* Left      */    terminal_string_left,
/* Right     */    terminal_string_right,
/* Enter     */    NULL,
/* Backspace */    terminal_string_backspace,
/* Delete    */    terminal_string_delete,
/* Tab       */    NULL,
/* Escape    */    NULL,
/* Home      */    terminal_string_start,
/* End       */    terminal_string_end,
/* Page Up   */    NULL,
/* Page Down */    NULL,
/* Insert    */    NULL
};



int main()
{

    init_terminal();

    pTerm_String_t ts = new_terminal_string();

    printf("-> ");
    fflush(stdout);

    while(1) {
        int c = terminal_getch();

        if (c > 32 && c < 127) {
            insert_terminal_char(ts,(char) c);

        }

        if (c == K_ENTER) {
            reset_terminal_string(ts);
            printf("\n-> ");
            fflush(stdout);
        }

        if (c == K_ESCAPE) {
            printf("\n");
            dump_terminal_string(ts);
            printf("-> %s",ts->str);
            fflush(stdout);
        }

        if (c == K_TAB) {
            insert_terminal_string(ts,"Hello");
        }

        if (TO_DO[c] != NULL) {TO_DO[c](ts);}
    }

    free_terminal_string(ts);

    close_terminal();


//    printf("%p\n",field);

    return 0;
}
