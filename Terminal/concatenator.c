#include <concatenator.h>
#include <dynamic-array.h>
#include <string.h>

char* concatenate_strings(char** strings, size_t count, char* space) {

    pDynamic_Arr_t arr = new_dynamic_array(sizeof(char));

    size_t i;
    for (i = 0; i < count; ++i) {
        if (i != 0) {add_array_elements(arr,space,strlen(space));}
        add_array_elements(arr,strings[i],strlen(strings[i]));
    }

    //ALWAYS ADD NULL-Terminator
    char end = 0;
    add_array_element(arr,&end);

    char* result = flush_dynamic_array(arr);
    free_dynamic_array(arr,NULL);

    return result;
}
