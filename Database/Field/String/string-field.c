#include <field.h>
#include <heap.h>
#include <stdlib.h>
#include <string.h>


//The private string structure
typedef struct {
	pMem_t index_mem;
	pMem_t data_mem;
	row_t next_row;		// Where to allocate a new string?
	row_t max_row;		// Used to determine when to resize strings
	pField_t me;    // Useful
} String_Private_t, *pString_Private_t;


//8 Bytes for String Heap Entry, doubly linked list
//  The next 4 kilobytes are associated with this heap
typedef struct {
    row_t pre;
    row_t next;
} String_Index_t, *pString_Index_t;

//The raw string data in a string (in 16 byte increments)
typedef struct {
    uint8_t arr[16];
} String_Data_t, *pString_Data_t;




//Private functions
static int insert_string(pField_t,const char*);
static int delete_string(pField_t,row_t);
static int put_string(pField_t,const char*, row_t);
static int get_string(pField_t,row_t, char**);
static void free_string_fp(pField_t);


//The parent field is NOT USED...
pField_t new_string_field(pField_t p) {

	pField_t field = calloc(1,sizeof(Field_t));
	pString_Private_t priv = calloc(1,sizeof(String_Private_t));

	field->type = STRING_FIELD;

	//All of the functions
	default_field_functions(field);
	field->insert = insert_string;
	field->delete = delete_string;
	field->put    = put_string;
	field->get    = get_string;
	field->free   = free_string_fp;

	//Private data
	priv->index_mem = new_memory(NULL, 1);      //Initial size: 1Kib Index (128 indexes)
	priv->data_mem =  new_memory(NULL, 2);      //              2Kib Data  (128 16-Byte strings)
	priv->next_row = 1;
	priv->max_row = 128;	/* We can use rows 1 to 128, but 129 > 128 */
    priv->me = field;
	field->private = (void*) priv;


    //Prepare the double-linked-list indexes
    row_t i;
    pString_Index_t arr = (pString_Index_t) priv->index_mem->addr;
    for (i = 0; i < priv->max_row; ++i) {
        arr[i].pre = i;
        arr[i].next = i+2;
    }

	return field;
}

static void free_string_fp(pField_t field) {
	if (!VALIDATE(field,STRING_FIELD)) {return;}

    pString_Private_t priv = (pString_Private_t) field->private;

	free_memory(priv->index_mem,1);
	free_memory(priv->data_mem,1);
	free(priv);
	free(field);
}


void dump_string_field(pField_t field) {
    if (!VALIDATE(field,STRING_FIELD)) {return;}

    pString_Private_t priv = field->private;
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;
    pString_Data_t data_arr = (pString_Data_t) priv->data_mem->addr;

    printf("Next Row: %u\n",priv->next_row);
    printf("Max Row: %u\n",priv->max_row);

    row_t i, j;
    for (i = 0; i < priv->max_row; ++i) {
        printf("%u|P:%u,N:%u\t",i+1,index_arr[i].pre,index_arr[i].next);
        for (j = 0; j < 16; ++j) {
            printf("%02hhX",data_arr[i].arr[j]);
        }

        printf("   ");
        for (j = 0; j < 16; ++j) {
            char c = data_arr[i].arr[j];
            if (c < 32 || c > 126) {printf(".");} else {
            printf("%c",c);}
        }
        printf("\n");
    }
}



// Test if a row is valid for a put, delete, or get
//
// For a row to be valid:
//   1. It must be within the data index (< max)
//   2. Be the start of a string, meaning that row->pre is a NULL row (0)
//   3. The row is not the start of freed data (priv->next_row)
//
// Returns 0 on success, or a negative on failure
static int is_valid_entry(pString_Private_t priv, row_t row) {
    if (!priv) {return BAD_OBJECT;}
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;

	if ((row > priv->max_row)                    /* Cond 1. */
        || (row == priv->next_row)               /* Cond 3. */
        || (index_arr[row-1].pre != NULL_ROW)) { /* Cond 2. */
            return INVALID_ROW;
    }

    return 0;
}




//Add another page into this string field
//  Automatically update the indexes as needed...
//
//  pre = What should be the previous row for the first entry in the new page
//          if NULL_ROW, then by default it is page_row - 1
//
//  Returns 0 on success, negative on failure
static int add_page(pString_Private_t priv, row_t prev) {

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





//Request the next 16-Byte entry from the string table, and update the list
//  Returns a NULL_ROW on failure
static row_t next_entry(pString_Private_t priv, row_t pre_row) {
    if (!priv) {return NULL_ROW;}

    //Only copy the first 16 bytes, for now
    pString_Index_t* index_arr = (pString_Index_t*) &(priv->index_mem->addr);
    row_t new_row = priv->next_row;

    if (new_row > priv->max_row) {

        //An out of bound row will only ever be max_row+1
        //  Otherwise, this row is a corrupted row, and should not be allowed
        if (new_row == priv->max_row+1) {
            if (add_page(priv,NULL_ROW) < 0) {return NULL_ROW;}
        } else {
            return NULL_ROW;	//This is a corrupted row. Return error!
        }
    }

    priv->next_row = (*index_arr)[new_row-1].next;
    (*index_arr)[new_row-1].next = NULL_ROW;
    (*index_arr)[priv->next_row-1].pre = NULL_ROW;

    //Update the previous row to point to me
    if (pre_row != NULL_ROW) {
        (*index_arr)[pre_row-1].next = new_row;
    }
    (*index_arr)[new_row-1].pre = pre_row;

    return new_row;
}


//Get the ending row of a linked list, no matter where you start.
//  Technically speaking, a row ends if:
//      1. (row->next or row->pre) == NULL_ROW
//      2. (row->next or row->pre) > priv->max_row
//
//  action: 0 (GET_END)   = Get the end row
//  action: 1 (GET_START) = Get the start row
//
//  Returns the start/end row, or NULL_ROW if invalid data is given
#define GET_END   0
#define GET_START 1
static row_t get_row_edge(pString_Private_t priv, row_t start, int action) {

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




// Find out how many rows I extend in a single direction
//
// action: 0 (GET_END)   = Get the end row
// action: 1 (GET_START) = Get the start row
//
// Returns the number of rows, or NULL_ROW if invalid data is given
static row_t get_row_length(pString_Private_t priv, row_t start, int action) {
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





static int insert_string(pField_t field,const char* str) {
	if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Data_t* data_arr = (pString_Data_t*) &(priv->data_mem->addr);   //Pointer MIGHT CHANGE during reallcation
    size_t len = strlen(str);

    //Enforce a minimum of 1 chunk (16 bytes)
    size_t chunks = (len == 0) ? 1 : (len >> 4) + ((len & 0xF) != 0);

    //When to resize the intermediary buffer
    if (field_force_buffer_size(chunks << 4) < 0) {return ALLOCATION_FAILURE;}

    //Stop copying at the first null terminator
    //  The string in static buf is NOT null terminated!!!
    char* tbuf = field_get_buffer();
    memccpy(tbuf,str,0,chunks << 4);

    //Run a loop to continuously copy the bytes
    row_t cur_row = next_entry(priv,0);
    row_t pre_row = cur_row;
    do {
        if (!pre_row) {return ALLOCATION_FAILURE;}
        memcpy(&((*data_arr)[pre_row-1].arr),tbuf,16);
        tbuf+=16;
        if (chunks > 1) {pre_row = next_entry(priv,pre_row);}
    } while(--chunks > 0);

    field->row = cur_row;
    return 0;
}



static int delete_string(pField_t field,row_t row) {
	if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Index_t* index_arr = (pString_Index_t*) &(priv->index_mem->addr);    // Pointer CAN CHANGE during this function

    int isValid = is_valid_entry(priv,row);
    if (isValid < 0) {return isValid;}

    //All we have to do to release it is to patch up the doubly linked list
    row_t end = get_row_edge(priv,row,GET_END);
    if (end == 0) {return LIST_ERROR;}

    (*index_arr)[end-1].next = priv->next_row;

    //Realloc if priv->next_row is too big
    if (priv->next_row > priv->max_row) {
        if (add_page(priv,end) < 0) {return LIST_ERROR;}
    } else {
        (*index_arr)[priv->next_row-1].pre = end;
    }

    priv->next_row = row;

	return 0;
}




//This is a fancy wrapper for delete + insert
static int put_string(pField_t field,const char* str, row_t row) {
    if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    int result;

    result = delete_string(field,row);
    if (result < 0) {return result;}

    result = insert_string(field,str);
    if (result < 0) {return result;}

    return 0;
}




static int get_string(pField_t field,row_t row, char** result) {
    if (!VALIDATE(field,STRING_FIELD)) {return BAD_OBJECT;}

    pString_Private_t priv = (pString_Private_t) field->private;
    pString_Index_t index_arr = (pString_Index_t) priv->index_mem->addr;    // Does not change
    pString_Data_t data_arr = (pString_Data_t) priv->data_mem->addr;    // Does not change

    int isValid = is_valid_entry(priv,row);
    if (isValid < 0) {return isValid;}

    row_t len = get_row_length(priv,row,GET_END);
    if (len == NULL_ROW) {return INVALID_ROW;}
    if (field_force_buffer_size(len) < 0) {return ALLOCATION_FAILURE;}

    //Now we copy into the buffer
    char* temp = field_get_buffer();
    while(len > 0) {
        memcpy(temp,data_arr[row-1].arr,16);    /* Every data entry is 16 bytes */

        row = index_arr[row-1].next;
        temp+=16;
        --len;
    }

    *result = field_get_buffer();

    return 0;
}
