#include <string-field.h>
#include <stdlib.h>

void free_string_fp(pField_t field) {
	if (!VALIDATE(field,STRING_FIELD)) {return;}

    pString_Private_t priv = (pString_Private_t) field->private;

	free_memory(priv->index_mem,1);
	free_memory(priv->data_mem,1);
	free(priv);
	free(field);
}
