#include <string-field.h>
#include <stdlib.h>

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
