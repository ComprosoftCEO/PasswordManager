#include <key-field.h>
#include <dynamic-array.h>
#include <stdlib.h>

void free_key_fp(pField_t field) {
    if (!VALIDATE(field,KEY_FIELD)) {return;}

    pKey_Private_t priv = (pKey_Private_t) field->private;

    free_memory(priv->index_mem,1);
    free_dynamic_array(priv->subs,NULL);
    free(priv);
    free(field);
}
