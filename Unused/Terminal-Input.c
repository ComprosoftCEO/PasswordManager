#include <Terminal.h>
#include <Escape-Codes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Define the getch command for linux systems
//  Be sure to call init_terminal beforehand, or this function will not work
#if defined(PLAT_LINUX)

    #include <unistd.h>
    #include <termios.h>
    #include <sys/ioctl.h>

    int getch(void) {
        return getchar();
    }

    // Test if there are any keys stored in stdin stream
    //  Returns 1 if there is a key left, 0 otherwise
    int kbhit(void) {
        struct termios original;
        tcgetattr(STDIN_FILENO, &original);

        struct termios term;
        memcpy(&term, &original, sizeof(term));

        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &term);

        int characters_buffered = 0;
        ioctl(STDIN_FILENO, FIONREAD, &characters_buffered);

        tcsetattr(STDIN_FILENO, TCSANOW, &original);

        int pressed = (characters_buffered != 0);

        return pressed;
    }

#elif defined(PLAT_WINDOWS)
    #include <conio.h>
#endif

typedef struct {
    int code;
    int *sequence;
} ESCAPE_t;



//Also tests for special input characters
int terminal_getch() {

    //Put all escape sequences here
    ESCAPE_t escapes[] = {
        {K_UP_ARROW,   (int[]) ESC_UP_ARROW},
        {K_DOWN_ARROW, (int[]) ESC_DOWN_ARROW},
        {K_LEFT_ARROW, (int[]) ESC_LEFT_ARROW},
        {K_RIGHT_ARROW,(int[]) ESC_RIGHT_ARROW},
        {K_ENTER,      (int[]) ESC_ENTER},
        {K_BACKSPACE,  (int[]) ESC_BACKSPACE},
        {K_DELETE,     (int[]) ESC_DELETE},
        {K_TAB,        (int[]) ESC_TAB},
        {K_ESCAPE,     (int[]) ESC_ESCAPE},
        {K_HOME,       (int[]) ESC_HOME},
        {k_END,        (int[]) ESC_END},
        {K_PAGE_UP,    (int[]) ESC_PAGE_UP},
        {K_PAGE_DOWN,  (int[]) ESC_PAGE_DOWN},
        {K_INSERT,     (int[]) ESC_INSERT}
    };

    int key;

    key = getch();
    if (key > 31 && key < 127) {return key;}

    //Because escape characters use more than 1 key, allocate an array
    //  that stores every character hit
    //
    //  Surely, we will never need more than 10 bytes for an escape sequence
    int* arr = malloc(sizeof(int) * 10);
    size_t arr_len = 10, arr_index = 1;
    arr[0] = key;

    while (kbhit()) {
        arr[arr_index++] = getch();

        //When to realloc
        if (arr_index >= arr_len) {
            arr_len+=10;
            arr = realloc(arr,arr_len * sizeof(int));
        }
    }

    //Figure out if it matches any escape key
    //  Cycle through all escape sequences, and test bytes until ESC_TERMINATOR is found
    //  Test both the bytes and the length (as 27 and 27,91,65 are different escape sequences)
    size_t esc, byte;
    for (esc = 0; esc < sizeof(escapes) / sizeof(escapes[0]); ++esc) {
        for (byte = 0; ; ++byte) {
            if ((escapes[esc].sequence[byte] == ESC_TERMINATOR) && (byte == arr_index)) {
                free(arr);
                return escapes[esc].code;
            }

            if (byte >= arr_index) {break;}
            if (arr[byte] != escapes[esc].sequence[byte]) {break;}
        }
    }

    free(arr);  /* Memory Leak Fix! */
    return K_UNKNOWN;
}
