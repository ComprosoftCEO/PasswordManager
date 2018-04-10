//The heap is responsible for loading and freeing memory
#include <PasswordManager.h>


pMem_t new_memory(pMem_t mem, size_t kilobytes) {

	int doNew = !mem;
	if (doNew) {mem = malloc(sizeof(Mem_t));}

	if (kilobytes == 0) {kilobytes = 1; /* Min size is 1KiB */}
	mem->size = (kilobytes << 10);
	mem->addr = calloc(mem->size,sizeof(uint8_t));

	if (mem->addr == NULL) {
		if (doNew) {free(mem);} return NULL;
	}

	return mem;
}



void free_memory(pMem_t mem, int freeObj) {

	if (mem == NULL) {return;}
	if (mem->addr != NULL) {
		free(mem->addr);
	}

	if (freeObj) {free(mem); return;}

	mem->addr = NULL;
	mem->size = 0;
}



int add_kilobyte(pMem_t mem, size_t count) {
	if (mem == NULL) {return -1;}
	if (count == 0) {return 0;}

	size_t new_size = mem->size + (count << 10);

	void* new_addr = realloc(mem->addr,new_size*sizeof(uint8_t));

	if (new_addr == NULL) {return -2;}
	mem->size = new_size;
	mem->addr = new_addr;

	return 0;
}



int resize_memory(pMem_t mem, size_t new_kilobytes) {
	if (mem == NULL) {return -1;}
	if (new_kilobytes == 0) {new_kilobytes = 1; /* Min size is 1KiB */ }

	size_t new_size = (new_kilobytes << 10);

	void* new_addr = realloc(mem->addr,new_size*sizeof(uint8_t));

	if (new_addr == NULL) {return -2;}
	mem->size = new_size;
	mem->addr = new_addr;

	return 0;
}



int write_to_file(pMem_t mem,FILE* file) {
	if (mem == NULL) {return -1;}
	if (file == NULL) {return -2;}

	//Do a loop because it might not write every byte the first time around
	size_t written = 0;
	do {
 		written += fwrite(((uint8_t*) mem->addr) + written,sizeof(uint8_t),
							(mem->size - written),file);

		if (ferror(file) != 0) {return -3;	/* That should have worked */ }
	} while(written < mem->size);

	return 0;
}



int import_from_file(pMem_t mem, FILE* file) {
	if (mem == NULL) {return -1;}
	if (file == NULL) {return -2;}

 	size_t read = 0;
	do {
 		read += fread(((uint8_t*) mem->addr) + read,sizeof(uint8_t),
						(mem->size - read),file);

		if (feof(file) != 0)   {return 1;	/* End of file, you decide if it is a problem */ }
		if (ferror(file) != 0) {return -3;	/* That should have worked */ }
	} while(read < mem->size);

	return 0;
}
