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



int main()
{

    init_terminal();

    while(1) {
        printf("%d\n",terminal_getch());
    }

    close_terminal();


//    printf("%p\n",field);

    return 0;
}
