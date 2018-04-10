//Private header for the key field internals
#ifndef KEY_FIELD_HEADER
#define KEY_FIELD_HEADER

#include <field.h>
#include <heap.h>
#include <dynamic-array.h>


//The private string structure
typedef struct {
	pMem_t index_mem;       // BTree structure
	row_t next_row;		    // Where to allocate a new string?
	row_t root_row;         // What row is the base of the tree
	row_t max_row;		    // Used to determine when to resize strings
    pField_t str_field;     // Holds the internal strings
    pDynamic_Arr_t subs;    // Sub fields that I own (Internal type: pField_t)
} Key_Private_t, *pKey_Private_t;


//Unbalanced binary tree structure
typedef struct {
    row_t left;         // Less Than
    row_t right;        // Greater Than
    row_t parent;       // Parent (also stores next key entry when not in use)
    row_t heapStart;    // Where does the string start in the string heap?
} BTree_Index_t, *pBTree_Index_t;



//Main public functions
int insert_key(pField_t field,const char* value);

int get_key(pField_t field,row_t row, char** result);

int next_key(pField_t field, char** result);

#endif // KEY_FIELD_HEADER Included
