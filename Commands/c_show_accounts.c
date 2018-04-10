#include <PasswordManager.h>
#include <command.h>

int c_show_accounts(size_t argc, const char** argv) {

    if (argc > 1) {
        printf("Too many arguments!\n");
        return CMD_RET_OK;
    }

    pField_t acc_field = db->account;
    char* str;
    acc_field->row = 0;
    row_t counter = 1;
    while(field_next(acc_field,&str) >= 0) {
        printf("%-5u %s\n",(unsigned int) counter, str);
        ++counter;
    }

    return CMD_RET_OK;
}
