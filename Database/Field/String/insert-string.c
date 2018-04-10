#include <string-field.h>
#include <string.h>


//Internal function to insert a value into a string field
int insert_string(pField_t field,const char* str) {
	if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Data_t* data_arr = (pString_Data_t*) &(priv->data_mem->addr);   //Pointer MIGHT CHANGE during reallcation
    size_t len = strlen(str);

    //Enforce a minimum of 1 chunk (16 bytes)
    size_t chunks = (len == 0) ? 1 : (len >> 4) + ((len & 0xF) != 0);

    //When to resize the intermediary buffer
    if (field_force_buffer_size(chunks << 4) < 0) {return ALLOCATION_FAILURE;}

    //Stop copying at the first null terminator
    //  The string in static buf is NOT null terminated!!!
    char* tbuf = field_get_buffer();
    memccpy(tbuf,str,0,chunks << 4);

    //Run a loop to continuously copy the bytes one chunk at a time
    row_t cur_row = string_new_entry(priv,0);
    row_t pre_row = cur_row;
    do {
        if (!pre_row) {return ALLOCATION_FAILURE;}
        memcpy(&((*data_arr)[pre_row-1].arr),tbuf,16);
        tbuf+=16;
        if (chunks > 1) {pre_row = string_new_entry(priv,pre_row);}
    } while(--chunks > 0);

    field->row = cur_row;
    return 0;
}
