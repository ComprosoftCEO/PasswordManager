#include <PasswordManager.h>
#include <Terminal.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {

    init_terminal();

    while(1) {
        char* buf = readline("-> ");
        if (buf) {
            if (strlen(buf) > 0) {add_history(buf);}

            Term_Tokens_t tkn = terminal_tokenizer(buf);

            size_t i;
            for(i = 0; i < tkn.count; ++i) {
                printf("Token %d: %s\n",i,tkn.tokens[i]);
            }

            free_tokenizer(tkn);
            free(buf);
        }
    }

    close_terminal();
    return 0;
}
