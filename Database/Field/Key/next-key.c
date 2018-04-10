#include <key-field.h>

//Result does not get modified if an error occurs
int next_key(pField_t field, char** result) {
    if (!VALIDATE(field,KEY_FIELD)) {return BAD_OBJECT;}

    pKey_Private_t priv = (pKey_Private_t) field->private;
    pBTree_Index_t bt = (pBTree_Index_t) priv->index_mem->addr;
    if ((field->row < 0) || (field->row > priv->max_row)) {
        return INVALID_ROW;
    }

    if (priv->root_row == NULL_ROW) {return END_OF_LIST;}
    if (field->row == NULL_ROW) {
        field->row = priv->root_row;
        goto finish;
    }

    //Get the next item in the tree
    if (bt[field->row-1].left != NULL_ROW) {
        field->row = bt[field->row-1].left;
        goto finish;
    }

try_right:
    if (bt[field->row-1].right != NULL_ROW) {
         field->row = bt[field->row-1].right;
         goto finish;
    }

try_parent:
    if (bt[field->row-1].parent != NULL_ROW) {

        //Which side did I come from ???
        row_t test_row = field->row;
        field->row = bt[field->row-1].parent;
        if (bt[field->row-1].left == test_row) {
            goto try_right;   //I came from the left side
        } else {
            goto try_parent;  //I came from the right side
        }
    } else {
        return END_OF_LIST;
    }

finish:
    if (bt[field->row-1].heapStart == 0) {return INVALID_ROW;}
    return field_get(priv->str_field,bt[field->row-1].heapStart,result);
}
