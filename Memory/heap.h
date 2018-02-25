#ifndef HEAP_HEADER
#define HEAP_HEADER

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

//Structure to store allocated memory
typedef struct {
	void* addr;		// Address
	size_t size;	// Size of the shared memory, in bytes
} Mem_t, *pMem_t;


//==========================================================
// Create a new memory object and allocate data in the heap
//==========================================================
//	If pMem_t is provided, it uses this to store the data
//	IF pMem_t is NULL, then allocate a new pMem_t object
//
//	Returns the object on success, or NULL on failure
pMem_t new_memory(pMem_t, size_t kilobytes);


//======================================================
// Release allocated data from the heap
//======================================================
// If freeObj = 1, then frees both the memory and pMem_t
void free_memory(pMem_t, int freeObj);


//===============================================
// Allocate more data to an existing object
//===============================================
//	Return 0 on success, or negative on failure
int add_kilobyte(pMem_t, size_t count);
int resize_memory(pMem_t, size_t new_kilobytes);



//================================================
// File operations with allocated memory
//================================================
//	Returns a negative number on error
//	Returns 1 if the end of the file is reached (potential error?)
//	Returns 0 on success!
int write_to_file(pMem_t,FILE*);
int import_from_file(pMem_t, FILE*);




#endif
