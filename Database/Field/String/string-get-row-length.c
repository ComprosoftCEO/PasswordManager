#include <string-field.h>

// Find out how many rows I extend in a single direction
//
// action: 0 (GET_END)   = Get the end row
// action: 1 (GET_START) = Get the start row
//
// Returns the number of rows, or NULL_ROW if invalid data is given
row_t string_get_row_length(pString_Private_t priv, row_t start, int action) {
    if (!priv) {return NULL_ROW;}
    if (start > priv->max_row || start == 0) {return NULL_ROW;}

    //Pointer won't change during this function
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;

    //Traverse until an edge is reached
    row_t count = 1;
    start = (action == GET_END) ? index_arr[start-1].next : index_arr[start-1].pre;
    while ((start != 0) && (start <= priv->max_row)) {
        ++count;
        start = (action == GET_END) ? index_arr[start-1].next : index_arr[start-1].pre;
    }

    /* We stumbled upon the free row, which terminates using row into unallocated data */
    if (start > priv->max_row) {return NULL_ROW;}
    return count;
}
