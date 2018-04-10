#include <PasswordManager.h>


pPM_Obj_t new_password_database() {

    pPM_Obj_t pm = calloc(1,sizeof(PM_Obj_t));
    pDB_Header_t head;
    if (!pm) {return NULL;}

    //Head is 1 kilobyte
    head = (pDB_Header_t) (pm->head = new_memory(NULL,1));
    if (!pm->head) {free(pm); return NULL;}

    //Create the fields
    if (!(pm->string = new_field(STRING_FIELD,NULL))) {goto error;}
    if (!(pm->account = new_field(KEY_FIELD,pm->string))) {goto error;}
    //if (!(pm->username = new_field(ITEM_FIELD,pm->account))) {goto error;}
    //if (!(pm->password = new_field(ITEM_FIELD,pm->account))) {goto error;}


    //Also configure the "to update" properties in the header
    pm->account->toUpdate  = &head->key_size;
    //pm->username->toUpdate = &head->uname_size;
    //pm->password->toUpdate = &head->pass_size;
    pm->string->toUpdate = &head->string_size;

    return pm;

error:
    if (pm) {
        if (pm->head) {free_memory(pm->head,1);}
        if (pm->account) {free_field(pm->account);}
        if (pm->username) {free_field(pm->username);}
        if (pm->password) {free_field(pm->password);}
        if (pm->string) {free_field(pm->string);}
        free(pm);
    }
    return NULL;
}


pDB_Header_t get_db_header(pPM_Obj_t pm) {
    if (!pm) {return NULL;}
    return (pDB_Header_t) pm->head->addr;
}
