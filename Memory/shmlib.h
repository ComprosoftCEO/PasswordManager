//======================================================
// shmlib.h -	Header file for Shared Memory library
//======================================================
#ifndef SHMLIB_HEADER
#define SHMLIB_HEADER

#include <stdint.h>
#include <stddef.h>

//Define the various errors:
#define ERROR_SHM_BAD_OBJECT    -1      //Shared memory object is null pointer
#define ERROR_SHM_CREATE        -2      //Error creating shared memory object
#define ERROR_SHM_RESIZE        -3      //Error resizing shared memory
#define ERROR_SHM_MEMMAP        -4      //Error mapping shared memory segment into memory
#define ERROR_SHM_MUNMAP        -5      //Error mapping shared memory out of memory
#define ERROR_SHM_OPEN          -6      //Unable to open shared memory segment
#define ERROR_SHM_CLOSE         -7      //Error closing shared memory file descriptor
#define ERROR_SHM_PROPS         -8      //Error reading shared memory properties
#define ERROR_SHM_KILL          -9      //Problem killing shared memory segment (unlink)
#define ERROR_SHM_INIT          -10     //Error initializing shared memory for new database


//Public Shared Memory Structure
//	Holds all shared memory data
typedef struct {
	void* addr;
	size_t size;
} Shm_t, *pShm_t;


pShm_t New_Shm_Obj(void);
void Free_Shm_Obj(pShm_t shm_obj);

int Shm_Create(pShm_t shm_obj, const char* name, const uint32_t segment, size_t size);
int Shm_Open(pShm_t shm_obj,const char* name, const uint32_t segment);
int Shm_Resize(pShm_t shm_obj, size_t newsize);
int Shm_Close(pShm_t shm_obj);
int Shm_Delete(pShm_t shm_obj);


#endif
