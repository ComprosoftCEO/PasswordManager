//POXIX shared memory library
#include <shmlib.h>
#include <Hash8.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>        /* For POSIX memory management constants */
#include <unistd.h>          /* For ftruncate */
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>			 /* For Error checking */



//=======================================
//Private Shared memory object for POSIX
//=======================================
typedef struct {
    void*    address;       //Address in the process space
    size_t   size;          //Actual size in bytes of the segment
    char*    name;	        //Actual name of the segment
    uint32_t segment;       //Number of the segment
    char*    posix_name;    //POSIX shared memory file name
    int      posix_fd;	    //POSIX shared memory file descriptor
} Shm_Obj_t, *pShm_Obj_t;




//Allocate a new shared memory object, set to all 0's
pShm_t New_Shm_Obj(void) {
	pShm_Obj_t shm = calloc(1,sizeof(Shm_Obj_t));
	return (pShm_t) shm;
}



//Release object from memory
void Free_Shm_Obj(pShm_t shm_obj) {

	pShm_Obj_t shm = (pShm_Obj_t) shm_obj;

	if (shm->name) {free(shm->name);}
	if (shm->posix_name) {free(shm->posix_name);}

	free(shm_obj);
}






//-----------------------------------------------
//		Build_POSIX_String(name,segment)
//-----------------------------------------------
// Run hash function on a string to generate
//	unique string for POSIX filename
//
// Returned string has the following format:
//	 [Forward Slash][Hash String][Null Terminator]
//
//	***Be sure to FREE the value returned***
//
//	Parameters:
//		name 	- Char name of the segment
//		segment - Same name can have many segments
//
//	Returns:
//		New string allocated in memory
static char* Build_POSIX_String(const char* name, const uint32_t segment) {

	char *hashString;
	char *hashVal, *retVal;

	//Hash String = [Name]-[Segment as 8-Digit hex][Null Terminator]
	hashString = malloc(sizeof(char) * (strlen(name) + 1 + 8 + 1));
	strcpy(hashString,name);
	strcat(hashString,"-");
	sprintf(hashString + strlen(name) + 1,"%02hhX%02hhX%02hhX%02hhX",
         (uint8_t) ((segment>>24) & 0xFF), (uint8_t) ((segment>>16) & 0xFF),
         (uint8_t) ((segment>>8) & 0xFF), (uint8_t) (segment & 0xFF)
    ) ;

    //TODO: Make the printf less ugly by using cross-platform uint32_t format

	//hashVal = Hash8_String(NULL,hashString,10);
	hashVal = strdup(hashString);
	free(hashString);

	//Returned string = /[Hash String][Null Terminator]
	retVal = (char*) malloc(sizeof(char) * (strlen(hashVal) + 2));
	retVal[0] = '/';
	strcpy(retVal+1,hashVal);
	free(hashVal);

	return retVal;
}





//Create a new POSIX shared memory segment
//
//  size = Size in byte
int Shm_Create(pShm_t shm_obj, const char* name, const uint32_t segment, size_t size) {

	//Ugly test for invalid (NULL) pointer
    if (!shm_obj) {return ERROR_SHM_BAD_OBJECT;}
    if (size == 0) {return ERROR_SHM_CREATE;}

    pShm_Obj_t shm = (pShm_Obj_t) shm_obj;	//Cast to private pointer
    shm->size = size;

	//Actual segment is run through hash function
	shm->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(shm->name,name);
	shm->posix_name = Build_POSIX_String(name,segment);


	//Create a new shared memory segment
	//--------------------------------------------------
	//O_RDWR  - Can read and write to memory segment
	//O_CREAT - Make a completely new segment
	//O_EXCL  - Throw an error if SHM already exists
	//0666	  - User, Group, and Other can read, write, but not execute
	shm->posix_fd = shm_open(shm->posix_name,O_RDWR | O_CREAT | O_EXCL, 0666);
	if (shm->posix_fd < 0) {return ERROR_SHM_CREATE;}


	//Resize the shared memory
	if (ftruncate(shm->posix_fd,size) < 0) {return ERROR_SHM_RESIZE;}


	//Load the memory into the current address space
	//--------------------------------------------------
	//NULL  	 - Kernel decides where to map shared memory
	//PROT_READ  - Can read from the shared memory
	//PROT_WRITE - Can write new dta to shared memory
	//MAP_SHARED - Changes are seen to other processes and master memory
	//0          - 0 Offset from the file
	shm->address = mmap(NULL,size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->posix_fd, 0);
	if (shm->address == MAP_FAILED) {return ERROR_SHM_MEMMAP;}

	return 0;   //We are cool
}



//Open shared memory that already exists
int Shm_Open(pShm_t shm_obj, const char* name, const uint32_t segment) {

	//Ugly test for invalid (NULL) pointer
	if (!shm_obj) {return ERROR_SHM_BAD_OBJECT;}

	pShm_Obj_t shm = (pShm_Obj_t) shm_obj;	//Cast to private pointer

	//Actual segment is run through hash function
	shm->name = malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(shm->name,name);
	shm->posix_name = Build_POSIX_String(name,segment);


	//Open an existing shared memory segment
	//--------------------------------------------------
	//O_RDWR  - Can read and write to memory segment
	//0666	  - User, Group, and Other can read, write, but not execute
	shm->posix_fd = shm_open(shm->posix_name,O_RDWR, 0666);
	if (shm->posix_fd < 0) {return ERROR_SHM_OPEN;}


	//Calculate the length
	//	struct stat = Structure used to get some information about file
	struct stat fstats;
	if (fstat(shm->posix_fd,&fstats) < 0) {return ERROR_SHM_PROPS;}
	shm->size = fstats.st_size;


	//Load the memory into the current address space
	//--------------------------------------------------
	//NULL  	 - Kernel decides where to map shared memory
	//PROT_READ  - Can read from the shared memory
	//PROT_WRITE - Can write new dta to shared memory
	//MAP_SHARED - Changes are seen to other processes and master memory
	//0          - 0 Offset from the file
	shm->address = mmap(NULL,shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->posix_fd, 0);
	if (shm->address == MAP_FAILED) {return ERROR_SHM_MEMMAP;}


	return 0;
}



//Resize the existing shared memory to match the new size
int Shm_Resize(pShm_t shm_obj, size_t newsize) {

	//Ugly test for invalid (NULL) pointer
	if (!shm_obj) {return ERROR_SHM_BAD_OBJECT;}

    pShm_Obj_t shm = (pShm_Obj_t) shm_obj;	//Cast to private pointer
    if (shm->address == NULL) {return ERROR_SHM_BAD_OBJECT;}
    if (shm->posix_fd < 0) {return ERROR_SHM_BAD_OBJECT;}

    //Resize the shared memory
	if (ftruncate(shm->posix_fd,newsize) < 0) {return ERROR_SHM_RESIZE;}

	//Unmap the shared memory first, then remap
    if (munmap(shm->address,shm->size) < 0) {return ERROR_SHM_MUNMAP;}

	//NULL  	 - Kernel decides where to map shared memory
	//PROT_READ  - Can read from the shared memory
	//PROT_WRITE - Can write new dta to shared memory
	//MAP_SHARED - Changes are seen to other processes and master memory
	//0          - 0 Offset from the file
	shm->size = newsize;
	shm->address = mmap(NULL,shm->size, PROT_READ | PROT_WRITE, MAP_SHARED, shm->posix_fd, 0);
	if (shm->address == MAP_FAILED) {return ERROR_SHM_MEMMAP;}

    return 0;
}



//Close an open shared memory segment
//	Does not free shm_obj
//
//To close shared memory:
//	1. Unmap
//	2. Close the file descriptor (not needed anymore)
int Shm_Close(pShm_t shm_obj) {

	//Ugly test for invalid (NULL) pointer(s)
	pShm_Obj_t shm = (pShm_Obj_t) shm_obj;
	if (!shm_obj) {return ERROR_SHM_BAD_OBJECT;}
	if (!shm->address || !shm->posix_name) {return ERROR_SHM_BAD_OBJECT;}

	if (munmap(shm->address,shm->size) < 0) {return ERROR_SHM_MUNMAP;}

	if (close(shm->posix_fd) < 0) {return ERROR_SHM_CLOSE;}

	//Free stuffs from memory
	free(shm->name);		shm->name=0;
	free(shm->posix_name);	shm->posix_name=0;
    shm->posix_fd = -1;
	shm->address = NULL;

	return 0;
}



//Completely whipe out shared memory
//
//To kill shared memory:
//	1. Unmap
//	2. Unlink (shm_unlink)
//	3. Close the file descriptor (not needed anymore)
int Shm_Delete(pShm_t shm_obj) {

	//Ugly test for invalid (NULL) pointer(s)
	pShm_Obj_t shm = (pShm_Obj_t) shm_obj;
	if (!shm_obj) {return ERROR_SHM_BAD_OBJECT;}
	if (!shm->address || !shm->posix_name) {return ERROR_SHM_BAD_OBJECT;}

	if (munmap(shm->address,shm->size) < 0) {return ERROR_SHM_MUNMAP;}

	if (shm_unlink(shm->posix_name) < 0) {return ERROR_SHM_KILL;}

	if (close(shm->posix_fd) < 0) {return ERROR_SHM_CLOSE;}

	//Free stuffs from memory
	free(shm->name);		shm->name=0;
	free(shm->posix_name);	shm->posix_name=0;
    shm->posix_fd = -1;
	shm->address = NULL;

	return 0;
}
