#include <PasswordManager.h>
#include <command.h>

//Enable edit mode
int c_add_account(size_t argc,const char** argv) {

    if (!param_test(2,argc,PARAM_EXACT)) {
        printf("Missing account name\n");
        return CMD_RET_OK;
    }

    pField_t name_field = db->account;
    int result = field_insert(name_field,argv[1]);

    if (result == DUPLICATE_KEY) {
        printf("Account already exists!\n");
    } else if (result < 0) {
        printf("Error creating the account! %d\n",result);
    }

    return CMD_RET_OK;
}
