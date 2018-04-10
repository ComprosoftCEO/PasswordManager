#include <key-field.h>
#include <stdlib.h>

//The parent field is the string field...
pField_t new_key_field(pField_t str_field) {
    pField_t field = calloc(1,sizeof(Field_t));
	pKey_Private_t priv = calloc(1,sizeof(Key_Private_t));

	field->type = KEY_FIELD;

	//All of the functions
	default_field_functions(field);
	field->insert = insert_key;
	//field->delete = delete_key;
	field->get    = get_key;
	field->next   = next_key;
	//field->free   = free_key_fp;
    //field->add_child = add_key_child;

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
