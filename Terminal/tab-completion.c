//This file handles all of the fancy tab completion
#include <Terminal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

//This is the command list used by tag generator
static pTerm_Command_t use_com;

static char **tab_completion(const char *, int, int);
static char *tab_generator(const char *, int);
static char *escape(const char *);
static int quote_detector(char *, int);


void init_tab_completion() {
    rl_attempted_completion_function = &tab_completion;
    rl_completer_quote_characters = "'\"";
    rl_completer_word_break_characters = " ";
    rl_char_is_quoted_p = &quote_detector;
}

//Returns an array of possible commands
static char ** tab_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 1;

    //What set of commands should I use
    char* temp_str = strdup(rl_line_buffer);
    temp_str[start] = 0;    //Only use the tokens leading up to this
    use_com = get_commands_list(temp_str);
    free(temp_str);

    return rl_completion_matches(text, tab_generator);
}

static char * tab_generator(const char *text, int state) {
    static int list_index, len;
    char *name;

    if (use_com == NULL) {return NULL;}

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = use_com[list_index++].command) != NULL) {
        if (rl_completion_quote_character) {
            name = strdup(name);
        } else {
            name = escape(name);
        }

        if (strncmp(name, text, len) == 0) {
            return name;
        } else {
            free(name);
        }
    }

    return NULL;
}


//Return a new string with escaped characters
static char * escape(const char *original) {
    size_t original_len;
    int i, j;
    char *escaped, *resized_escaped;

    original_len = strlen(original);

    if (original_len > SIZE_MAX / 2) {
        fprintf(stderr, "string too long to escape");
        return NULL;
    }

    if ((escaped = malloc(2 * original_len + 1)) == NULL) {
        return NULL;
    }

    for (i = 0, j = 0; i < original_len; ++i, ++j) {
      if (original[i] == ' ') {
          escaped[j++] = '\\';
      }
      escaped[j] = original[i];
    }
    escaped[j] = '\0';

    /* Don't forget the null terminator */
    if ((resized_escaped = realloc(escaped, j+1)) == NULL) {
        free(escaped);
        resized_escaped = NULL;
        return NULL;
    }

    return resized_escaped;
}

//Test if a quote exists, accounting for double (\\)
static int quote_detector(char *line, int index) {
    return (
        index > 0 &&
        line[index - 1] == '\\' &&
        !quote_detector(line, index - 1)
    );
}
