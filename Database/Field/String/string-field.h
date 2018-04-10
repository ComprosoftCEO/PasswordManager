//Private header for the string field internals
#ifndef STRING_FIELD_HEADER
#define STRING_FIELD_HEADER

#include <field.h>
#include <heap.h>

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


//Main functions
pField_t new_string_field(pField_t p);
void free_string_fp(pField_t field);
int insert_string(pField_t field,const char* str);
int delete_string(pField_t field,row_t row);
int get_string(pField_t field,row_t row, char** result);
int put_string(pField_t field,const char* str, row_t row);



#define GET_END   0
#define GET_START 1
row_t string_get_row_length(pString_Private_t priv, row_t start, int action);
row_t string_get_row_edge(pString_Private_t priv, row_t start, int action);

int string_is_valid_entry(pString_Private_t priv, row_t row);


//Add another page into this string field
//  For correctly updating indexes, use prev with the
int string_add_page(pString_Private_t priv, row_t prev);

//Add space for a new string "chunk" (of 16 bytes)
row_t string_new_entry(pString_Private_t priv, row_t pre_row);

//Print out the contents of the string field
void dump_string_field(pField_t field);

#endif // STRING_FIELD_HEADER Included

