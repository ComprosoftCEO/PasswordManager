//Main header file for the password manager
#ifndef PASSWORD_MANAGER_HEADER
#define PASSWORD_MANAGER_HEADER

//Standard headers to always include
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

//Encryption algorithms
#include <aes.h>
#include <sha256.h>

//My headers
typedef struct PM_Obj_t* pPM_Obj_t;
#include <heap.h>
#include <database.h>
#include <field.h>
//#include <Terminal.h>

#define MAGIC_WORD 0x00504153534d414e   /* PASSMAN */
#define MAJOR_VERSION 1
#define MINOR_VERSION 0


//Password Manager Connection Object
typedef struct PM_Obj_t {
    pMem_t head;
    pField_t account;
    pField_t username;
    pField_t password;
    pField_t string;
} PM_Obj_t, *pPM_Obj_t;


#endif // PASSWORD_MANAGER_HEADER Included
