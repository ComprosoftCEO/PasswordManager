#include <string-field.h>

//Get the ending row of a linked list, no matter where you start.
//  Technically speaking, a row ends if:
//      1. (row->next or row->pre) == NULL_ROW
//      2. (row->next or row->pre) > priv->max_row
//
//  action: 0 (GET_END)   = Get the end row
//  action: 1 (GET_START) = Get the start row
//
//  Returns the start/end row, or NULL_ROW if invalid data is given

row_t string_get_row_edge(pString_Private_t priv, row_t start, int action) {

    if (!priv) {return NULL_ROW;}
    if (start > priv->max_row || start == 0) {return NULL_ROW;}

    //Pointer won't change during this function
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;

    //Traverse until an edge is reached
    row_t next = (action == GET_END) ? index_arr[start-1].next : index_arr[start-1].pre;
    while ((next != 0) && (next <= priv->max_row)) {
        start = next;
        next = (action == GET_END) ? index_arr[start-1].next : index_arr[start-1].pre;
    }

    /* We stumbled upon the free row, which terminates using row into unallocated data */
    if (next > priv->max_row) {return NULL_ROW;}
    return start;
}
