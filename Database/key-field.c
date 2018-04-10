#include <field.h>
#include <heap.h>
#include <dynamic-array.h>
#include <stdlib.h>
#include <string.h>


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



//Private functions
static int insert_key(pField_t,const char*);
static int delete_key(pField_t,row_t);
//static int put_key(pField_t,const char*, row_t);
static int get_key(pField_t,row_t, char**);
static void free_key_fp(pField_t);
static int add_key_child(pField_t);
static int key_next(pField_t,char**);



//The parent field is the string field...
pField_t new_key_field(pField_t str_field) {
    pField_t field = calloc(1,sizeof(Field_t));
	pKey_Private_t priv = calloc(1,sizeof(Key_Private_t));

	field->type = KEY_FIELD;

	//All of the functions
	default_field_functions(field);
	field->insert = insert_key;
	field->delete = delete_key;
	field->get    = get_key;
	field->next   = key_next;
	field->free   = free_key_fp;
    field->add_child = add_key_child;

	//Private data
	priv->index_mem = new_memory(NULL, 2);      //Initial size: 2Kib Index (128 indexes, 16 bytes each)
	priv->next_row = 1;
	priv->root_row = 0;
	priv->max_row = 128;	/* We can use rows 1 to 128 */
	priv->str_field = str_field;
	priv->subs = new_dynamic_array(sizeof(pField_t));
	field->private = (void*) priv;

	row_t i;
	pBTree_Index_t bt = (pBTree_Index_t) priv->index_mem->addr;
	for (i = 0; i < priv->max_row; ++i) {
        bt[i].parent = i+2;
	}

	return field;
}




static int insert_key(pField_t field,const char* value) {
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


static int delete_key(pField_t field,row_t row) {return 0;}


static int get_key(pField_t field,row_t row, char** result) {

    if (!VALIDATE(field,KEY_FIELD)) {return BAD_OBJECT;}

    pKey_Private_t priv = (pKey_Private_t) field->private;
    if ((row == NULL_ROW) || (row <= 0) || (row > priv->max_row)) {
        return INVALID_ROW;
    }

    pBTree_Index_t bt = (pBTree_Index_t) priv->index_mem->addr;
    if (bt[row-1].heapStart == 0) {return INVALID_ROW;}

    return field_get(priv->str_field,bt[row-1].heapStart,result);
}


//Result does not get modified if an error occurs
static int key_next(pField_t field, char** result) {
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




static void free_key_fp(pField_t field) {return;}
static int add_key_child(pField_t field) {return 0;}
