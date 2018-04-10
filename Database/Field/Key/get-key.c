#include <key-field.h>

int get_key(pField_t field,row_t row, char** result) {

    if (!VALIDATE(field,KEY_FIELD)) {return BAD_OBJECT;}

    pKey_Private_t priv = (pKey_Private_t) field->private;
    if ((row == NULL_ROW) || (row <= 0) || (row > priv->max_row)) {
        return INVALID_ROW;
    }

    pBTree_Index_t bt = (pBTree_Index_t) priv->index_mem->addr;
    if (bt[row-1].heapStart == 0) {return INVALID_ROW;}

    return field_get(priv->str_field,bt[row-1].heapStart,result);
}
