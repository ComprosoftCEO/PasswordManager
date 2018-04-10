#include <key-field.h>
#include <string.h>

int insert_key(pField_t field,const char* value) {
    if (!VALIDATE(field,KEY_FIELD)) {return BAD_OBJECT;}

    pKey_Private_t priv = (pKey_Private_t) field->private;
    pBTree_Index_t bt = (pBTree_Index_t) priv->index_mem->addr;

    //When to add a page???
    //  2 KiB = 128 Entries (16 bytes, each)
    if (priv->next_row > priv->max_row) {
        if (add_kilobyte(priv->index_mem,2) < 0) {
            return ALLOCATION_FAILURE;
        }
        priv->max_row+=128;
    }

    //Find where to put into the binary tree
    //  Do this first to avoid duplicates....
    row_t *update = &priv->root_row;
    row_t parent = 0;
    row_t temp_row = priv->root_row;

    while (temp_row != 0) {

        char* temp_str;
        int result = field_get(field,temp_row,&temp_str);
        if (result < 0) {return TREE_ERROR;}

        int cmp = strcmp(value,temp_str);
        if (cmp < 0) {
            update = &(bt[temp_row-1].left);
            parent = temp_row;
            temp_row = bt[temp_row-1].left;
        } else if (cmp > 0) {
            update = &(bt[temp_row-1].right);
            parent = temp_row;
            temp_row = bt[temp_row-1].right;
        } else {
            field_flush_buffer();
            return DUPLICATE_KEY;
        }
    }


    //Okay, our string is unique.
    if (field_insert(priv->str_field,value) < 0) {
        return INSERT_ERROR;
    }

    row_t new_row = priv->next_row;
    priv->next_row = bt[new_row-1].parent;    // Parent stores the next row
    bt[new_row-1].heapStart = priv->str_field->row;
    bt[new_row-1].left = NULL_ROW;
    bt[new_row-1].right = NULL_ROW;
    bt[new_row-1].parent = parent;
    *update = new_row;

    field->row = new_row;

    return 0;
}
