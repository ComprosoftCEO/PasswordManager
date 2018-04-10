#include <string-field.h>

// Test if a row is valid for a put, delete, or get
//
// For a row to be valid:
//   1. It must be within the data index (< max)
//   2. Be the start of a string, meaning that row->pre is a NULL row (0)
//   3. The row is not the start of freed data (priv->next_row)
//
// Returns 0 on success, or a negative on failure
int string_is_valid_entry(pString_Private_t priv, row_t row) {
    if (!priv) {return BAD_OBJECT;}
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;

	if ((row > priv->max_row)                    /* Cond 1. */
        || (row == priv->next_row)               /* Cond 3. */
        || (index_arr[row-1].pre != NULL_ROW)) { /* Cond 2. */
            return INVALID_ROW;
    }

    return 0;
}
