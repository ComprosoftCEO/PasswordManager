#include <string-field.h>

//This is a fancy wrapper for delete + insert
int put_string(pField_t field,const char* str, row_t row) {
    if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    int result;

    result = delete_string(field,row);
    if (result < 0) {return result;}

    result = insert_string(field,str);
    if (result < 0) {return result;}

    return 0;
}
