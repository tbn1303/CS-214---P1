#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
#define ALIGN8(x) (((x) + 7) & ~7)
#define MIN_CHUNK 8

//Static heap with 8-byte alignment
static union{
	char bytes[MEMLENGTH];
	double not_used;  // ensures 8-byte alignment
} heap;

// Chunk metadata
typedef struct chunk{
	int size; //Payload size
	int inuse; //1 = allocated, 0 = free
} chunk;

static chunk *free_chunk = NULL;
static int heap_initialized = 0;

// Leak detection
static void leak_detection(){
	size_t total_bytes = 0;
	int total_objects = 0;
	
	chunk *c = free_chunk;

	while((char *) c < heap.bytes + MEMLENGTH){
		if(c->inuse){
			total_bytes += c->size;
			total_objects++;
		}

		c = (chunk *)((char *)c + sizeof(chunk) + c->size);
	}
	
	fprintf(stderr, "mymalloc: %zu bytes leaked in %d objects\n", total_bytes, total_objects);
}

// Clean heap at exit
static void clean_heap(){
	leak_detection();
	memset(heap.bytes, 0, MEMLENGTH); //Set all bytes to 0 in the heap
	free_chunk = NULL;
	heap_initialized = 0;
}

// Initialize heap
static void initialize_heap(){
	atexit(clean_heap);
	free_chunk = (chunk *)heap.bytes;
	free_chunk->size = MEMLENGTH - sizeof(chunk);
	free_chunk->inuse = 0;
	heap_initialized = 1;
}

// Coalesce free chunks
static void coalesce_chunk(){
	chunk *curr = (chunk *)heap.bytes;
	while((char *)curr < heap.bytes + MEMLENGTH){
		chunk *next = (chunk *)((char *)curr + sizeof(chunk) + curr->size); //Temporary pointer to next chunk

		//If next goes beyond heap, stop merging
		if((char *)next >= heap.bytes + MEMLENGTH) break;

		//Merge curr and next chunk if free
		if(!curr->inuse && !next->inuse){
			curr->size += sizeof(chunk) + next->size;
		}

		//Move to next chunk
		else{
			curr = next;
		}
	}
}

// Memory allocation
void *mymalloc(size_t size, char *file, int line){
	//Initialize heap
	if(!heap_initialized)
		initialize_heap();

	size = ALIGN8(size); //Round size to multiple of 8
	chunk *curr = (chunk *)heap.bytes;

	while((char *)curr < heap.bytes + MEMLENGTH){
		if(!curr->inuse && size <= (size_t)curr->size){
			
			// Split heap if leftover space is enough for a new chunk
			if((size_t)curr->size - size >= sizeof(chunk) + MIN_CHUNK){
				chunk *remainder = (chunk *)((char *)curr + sizeof(chunk) + size);
				remainder->size = curr->size - sizeof(chunk) - size;
				remainder->inuse = 0;				
				curr->size = size;
			}

			curr->inuse = 1;
			return (char *)curr + sizeof(chunk); // return pointer to user data
		}

		curr = (chunk *)((char *)curr + sizeof(chunk) + curr->size);
	}
	
	fprintf(stderr, "mymalloc: cannot allocate %zu bytes (%s:%d)\n", size, file, line);
	
	return NULL;
}

//Memory deallocation
void myfree(void *ptr, char* file, int line){
	if(!heap_initialized) initialize_heap();
	if(!ptr) return;
	
	//Check if pointer belongs to heap
	if((char *)ptr < heap.bytes || (char *)ptr >= heap.bytes + MEMLENGTH){
		fprintf(stderr, "free: invalid pointer (%s:%d)\n", file, line);
		exit(2);
       	}
	
	//Get pointer to metadata
	chunk *c = (chunk *)((char *)ptr - sizeof(chunk));
	
	if(!c->inuse){
		fprintf(stderr, "free: double free (%s:%d)\n", file, line);
		exit(2);
	}
	
	c->inuse = 0;
	coalesce_chunk();
}
