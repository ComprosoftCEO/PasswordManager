#include <string-field.h>


int delete_string(pField_t field,row_t row) {
	if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Index_t* index_arr = (pString_Index_t*) &(priv->index_mem->addr);    // Pointer CAN CHANGE during this function

    int isValid = string_is_valid_entry(priv,row);
    if (isValid < 0) {return isValid;}

    //All we have to do to release it is to patch up the doubly linked list
    row_t end = string_get_row_edge(priv,row,GET_END);
    if (end == 0) {return LIST_ERROR;}

    (*index_arr)[end-1].next = priv->next_row;

    //Realloc if priv->next_row is too big
    if (priv->next_row > priv->max_row) {
        if (string_add_page(priv,end) < 0) {return LIST_ERROR;}
    } else {
        (*index_arr)[priv->next_row-1].pre = end;
    }

    priv->next_row = row;

	return 0;
}
