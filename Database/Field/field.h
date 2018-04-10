#ifndef FIELD_HEADER
#define FIELD_HEADER

#include <stdint.h>
#include <stddef.h>

#define NULL_ROW 0
typedef uint32_t row_t;     //32-Bit row index


#define EMPTY_FIELD    0
#define STRING_FIELD   1
#define KEY_FIELD      2
#define ITEM_FIELD     3

#define VALIDATE(FIELD,TYPE) ((FIELD != NULL) && (FIELD->type == TYPE))

//Error codes
#define FUNCTION_NOT_SUPPORTED -1   /* Funciton not supported by the field type */
#define BAD_OBJECT             -2   /* Null pointer passed */
#define ALLOCATION_FAILURE     -3   /* Unable to allocate or resize the memory */
#define INVALID_ROW            -4
#define LIST_ERROR             -5   /* The list indexes are broken in some way */
#define INSERT_ERROR           -6   /* Failed to insert the string in a key or item */
#define TREE_ERROR             -7   /* Error parsing the tree */
#define DUPLICATE_KEY          -8   /* Keys cannot have duplicates */
#define END_OF_LIST            -9   /* Not actually an error, just reached the end of Next*/


// Intermediary buffers for copying data
// ****These should be flushed after use to wipe out any decrypted data****
//extern char* static_field_buf;       // Intermediary string buffer, used for encryption & decryption
//extern size_t static_field_size;      // Number of bytes in buffer


//Structure for the database fields
typedef struct FIELD_STRUCT {
	row_t    row;
	uint32_t type;	//String, key, item, etc. (do not modify!!!)
	uint32_t* toUpdate;

	//Public functions for this field structure
	int (*insert)(struct FIELD_STRUCT*,const char*);
	int (*delete)(struct FIELD_STRUCT*,row_t);
	int (*put)(struct FIELD_STRUCT*,const char*, row_t);
	int (*get)(struct FIELD_STRUCT*,row_t,char** result);
	int (*next)(struct FIELD_STRUCT*,char** result);
	int (*pre)(struct FIELD_STRUCT*,char** result);
    int (*find)(struct FIELD_STRUCT*,const char* search);

	int (*add_child)(struct FIELD_STRUCT*);     //Only used with keys...
	void (*free)(struct FIELD_STRUCT*);			//Free the object from memory

	// "Keep your privates private"
	//   --Mr. Bobb, AP CS Teacher
	void* private;
} Field_t, *pField_t;


//Field functions
pField_t new_field(uint32_t type, pField_t parent);

int field_insert(pField_t field,const char* val);
int field_delete(pField_t field,row_t row);
int field_put(pField_t field, const char* str, row_t row);
int field_get(pField_t field, row_t row, char** result);
int field_next(pField_t field, char** result);
int field_pre(pField_t field, char** result);
int field_find(pField_t field, char* search);
void free_field(pField_t);

//Initialize functions that return an "Invalid Function" error
void default_field_functions(pField_t);


//Fields should use an intermediary buffer for storing data
//   The intermediary buffer is good for encrypting and decrypting data
char* field_get_buffer();
size_t field_get_buffer_size();
int field_force_buffer_size(size_t new_size);   //Guarantee at least size bytes in the buffer
void field_flush_buffer();                      //Load all 0's into the buffer (for security)


//Use the function new_field, not these functions
pField_t new_string_field(pField_t);
pField_t new_key_field(pField_t);
pField_t new_item_field(pField_t);


void dump_string_field(pField_t field);

#endif
