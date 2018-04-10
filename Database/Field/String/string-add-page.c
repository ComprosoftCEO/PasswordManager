#include <string-field.h>
#include <heap.h>

//Add another page into this string field
//  Automatically update the indexes as needed...
//
//  pre = What should be the previous row for the first entry in the new page
//          if NULL_ROW, then by default it is page_row - 1
//          (This is used to update the internal row indexes)
//
//  Returns 0 on success, negative on failure
int string_add_page(pString_Private_t priv, row_t prev) {

    if (!priv) {return BAD_OBJECT;}
    if (prev > priv->max_row) {return INVALID_ROW;} /* Needs to be in already allocated data */


    row_t max = priv->max_row + 128;    //Every kilobyte adds 8 rows
    pString_Index_t* index_arr = (pString_Index_t*) &(priv->index_mem->addr);

    //Short-Circuit if we don't need to add more
    if ((priv->index_mem->size < (max << 4) /* Index uses 1 KiB */) &&
        (add_kilobyte(priv->index_mem,1) < 0)) {return ALLOCATION_FAILURE;}

    if ((priv->data_mem->size < (max << 5) /* Data Mem uses 2 KiB */ ) &&
        (add_kilobyte(priv->data_mem,2) < 0))  {return ALLOCATION_FAILURE;}

    //Be sure to index the new page, too
    row_t i;
    for (i = priv->max_row; i < max; ++i) {
        (*index_arr)[i].next = i+2;
        (*index_arr)[i].pre = i;
    }

    if (prev != NULL_ROW) {(*index_arr)[priv->max_row].pre = prev;}

    priv->max_row = max;
    if (priv->me->toUpdate) {(*priv->me->toUpdate)+=1;}   /* Note size update in db header */

    return 0;
}
