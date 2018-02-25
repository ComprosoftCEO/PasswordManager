#include <PasswordManager.h>


//Unbalanced binary tree structure
typedef struct {
    row_t left;         // Less Than
    row_t right;        // Greater Than
    row_t parent;       // Parent (also stores next key entry when not in use)
    row_t heapStart;    // Where does the string start in the string heap?
} BTree_Index_t, *pBTree_Index_t;



pField_t new_key_field(pField_t parent) {
    return NULL;
}
