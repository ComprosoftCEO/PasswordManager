#include <PasswordManager.h>
#include <terminal.h>
#include <tokenizer.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {

    init_terminal();

    while(1) {
        char* buf = readline("-> ");
        if (buf) {
            if (strlen(buf) > 0) {add_history(buf);}

            Tokens_t tkn = tokenize_string(buf);

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
