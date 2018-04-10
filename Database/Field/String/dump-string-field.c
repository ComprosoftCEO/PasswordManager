#include <string-field.h>
#include <stdio.h>

//Print out the contents of the string field
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
