#include <PasswordManager.h>

typedef struct {
    uint32_t index;
    pField_t (*fun)(pField_t);
} FIELD_ENTRY_t;


pField_t new_field(uint32_t type, pField_t parent) {

    //List of field entries
    FIELD_ENTRY_t ALL_ENTRIES[] = {
        {STRING_FIELD,  new_string_field  },
        {KEY_FIELD,     new_key_field     },
        {ITEM_FIELD,    new_item_field    }
    };
    size_t i, CNT = sizeof(ALL_ENTRIES) / sizeof(ALL_ENTRIES[0]);

    pField_t field = NULL;
    for (i = 0; i < CNT; ++i) {
        if (type == ALL_ENTRIES[i].index) {
            field = ALL_ENTRIES[i].fun(parent);
            if (!field) {return NULL;}
            break;
        }
    }

    return field;
}


//Default functions that do nothing
static int default_insert(pField_t f,const char* s) {return FUNCTION_NOT_SUPPORTED;}
static int default_delete(pField_t f,row_t r)       {return FUNCTION_NOT_SUPPORTED;}
static int default_get(pField_t f,row_t r, char** c)      {return FUNCTION_NOT_SUPPORTED;}
static int default_put(pField_t f,const char* s, row_t r) {return FUNCTION_NOT_SUPPORTED;}
static int default_next(pField_t f) {return FUNCTION_NOT_SUPPORTED;}
static int default_pre(pField_t f) {return FUNCTION_NOT_SUPPORTED;}
static int default_add(pField_t f)  {return FUNCTION_NOT_SUPPORTED;}
static void default_free(pField_t f) {
    if (!f) {return;}
    free(f);
}


void default_field_functions(pField_t field) {
    if (!field) {return;}

    field->insert = default_insert;
    field->delete = default_delete;
    field->get = default_get;
    field->put = default_put;
    field->next = default_next;
    field->pre = default_pre;
    field->add_child = default_add;
    field->free = default_free;
}

void free_field(pField_t field) {
    if (!field) {return;}
    field->free(field);
}


int field_insert(pField_t field,const char* val) {
    if (!field) {return BAD_OBJECT;}
    return field->insert(field,val);
}

int field_delete(pField_t field,row_t row) {
    if (!field) {return BAD_OBJECT;}
    return field->delete(field,row);
}

int field_put(pField_t field, const char* str, row_t row) {
    if (!field) {return BAD_OBJECT;}
    return field->put(field,str,row);
}

int field_get(pField_t field, row_t row, char** result) {
    if (!field) {return BAD_OBJECT;}
    return field->get(field,row,result);
}

int field_next(pField_t field) {
    if (!field) {return BAD_OBJECT;}
    return field->next(field);
}

int field_pre(pField_t field) {
    if (!field) {return BAD_OBJECT;}
    return field->pre(field);
}
