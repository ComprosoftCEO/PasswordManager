//Handles the internal buffer used when passing data between the the field and the database
//  Makes the encryption process easier to manage
#include <PasswordManager.h>


//Intermediary buffers to use when writing to the database
// ****These should be flushed after use to wipe out any decrypted data****
static char* static_field_buf = NULL;     // Intermediary string buffer, used for encryption & decryption
static size_t static_field_size = 0;      // Number of bytes in buffer


size_t field_get_buffer_size() {return static_field_size;}
char* field_get_buffer() {return static_field_buf;}

//Guarantee that the field buffer has at least this many bytes (it may have more)
//  Returns negative on failure
int field_force_buffer_size(size_t new_size) {
    char* temp_ptr = NULL;

    if (new_size > static_field_size) {
        static_field_size = new_size;
        if (static_field_buf) {temp_ptr = realloc(static_field_buf,static_field_size* sizeof(char));}
        else {temp_ptr = malloc(static_field_size * sizeof(char));}

        if (temp_ptr == NULL) {return -1;}
        static_field_buf = temp_ptr;
    }

    return 0;
}


void field_flush_buffer() {
    memset(static_field_buf,0,static_field_size);
}
