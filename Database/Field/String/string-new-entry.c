#include <string-field.h>

//Request the next 16-Byte entry from the string table, and update the list
//  Returns a NULL_ROW on failure
row_t string_new_entry(pString_Private_t priv, row_t pre_row) {
    if (!priv) {return NULL_ROW;}

    //Only copy the first 16 bytes, for now
    pString_Index_t* index_arr = (pString_Index_t*) &(priv->index_mem->addr);
    row_t new_row = priv->next_row;

    if (new_row > priv->max_row) {

        //An out of bound row will only ever be max_row+1
        //  Otherwise, this row is a corrupted row, and should not be allowed
        if (new_row == priv->max_row+1) {
            if (string_add_page(priv,NULL_ROW) < 0) {return NULL_ROW;}
        } else {
            return NULL_ROW;	//This is a corrupted row. Return error!
        }
    }

    priv->next_row = (*index_arr)[new_row-1].next;
    (*index_arr)[new_row-1].next = NULL_ROW;
    (*index_arr)[priv->next_row-1].pre = NULL_ROW;

    //Update the previous row to point to me
    if (pre_row != NULL_ROW) {
        (*index_arr)[pre_row-1].next = new_row;
    }
    (*index_arr)[new_row-1].pre = pre_row;

    return new_row;
}
