#include <string-field.h>
#include <string.h>

int get_string(pField_t field,row_t row, char** result) {
    if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;    // Does not change
    pString_Data_t data_arr = (pString_Data_t) priv->data_mem->addr;    // Does not change

    int isValid = string_is_valid_entry(priv,row);
    if (isValid < 0) {return isValid;}

    //Allocate enough space in the buffer
    row_t len = string_get_row_length(priv,row,GET_END);
    if (len == NULL_ROW) {return INVALID_ROW;}
    if (field_force_buffer_size(len) < 0) {return ALLOCATION_FAILURE;}

    //Now we copy into the buffer
    char* temp = field_get_buffer();
    while(len > 0) {
        memcpy(temp,data_arr[row-1].arr,16);    /* Every data entry is 16 bytes */

        row = index_arr[row-1].next;
        temp+=16;
        --len;
    }

    *result = field_get_buffer();

    return 0;
}
