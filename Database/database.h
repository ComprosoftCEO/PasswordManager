//Functions associated with the structure of the underlying database
#ifndef DATABASE_HEADER
#define DATABASE_HEADER


//Database Header
typedef struct {
    uint64_t magic;     // Used to determine if this is a legit password manager file
    uint32_t maj_ver;   // Major Version
    uint32_t min_ver;   // Minor Version

    uint32_t key_size;     // How big is the heap for the key field (in Kilobytes)
    uint32_t uname_size;   // How big is the user name field (in Kilobytes)
    uint32_t pass_size;    // How big is the password field (in Kilobytes)
    uint32_t string_size;  // How big is the string index (use this to derive string data, as well)

    /* 1 Kilobyte of space total in header */

} DB_Header_t, *pDB_Header_t;


pPM_Obj_t new_password_database();

pDB_Header_t get_db_header(pPM_Obj_t pm);




#endif // DATABASE_HEADER Included
