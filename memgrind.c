#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

#define RUNS 50
#define ALLOCATES 120

//Allocate and deallocate immediately 1-byte object, 120 times
void task_1(void){
	for(int i = 0; i < ALLOCATES; i++){
		char *p = malloc(1);
		free(p);
	}
}

//Allocate of 120 1-byte objects, then deallocate them
void task_2(void){
	char *obj[ALLOCATES];
	for(int i = 0; i < ALLOCATES; i++){
		obj[i] = malloc(1);
	}

	for(int i = 0; i < ALLOCATES; i++){
		free(obj[i]);
	}
}

//Randomly chose to allocate 120 objects or deallocate a random object
void task_3(void){
	char *obj[ALLOCATES] = {0};
	int total_allocated = 0; //Count total allocated object (we need to allocate 120 objects in this case)
	int total_freed = 0; //Count total freed object (for debug purpose)

	while(total_allocated < ALLOCATES){
		int choice = rand() % 2;

		//Allocate object
		if(choice == 0){
			for(int i = 0; i < ALLOCATES; i++){
				//Check if object has not been allocated
				if(!obj[i]){
					obj[i] = malloc(1);
					total_allocated++;
					break;
				}
			}
		}

		else{
			//Free random object (if any)
			int index = rand() % ALLOCATES;

			//Check if object has been allocated
			if(obj[index]){
				free(obj[index]);
				obj[index] = NULL;
				total_freed++;
				break;
			}
		}
	}

	//Deallocate all remaining objects
	for(int i = 0; i < ALLOCATES; i++){
		if(obj[i]){
			free(obj[i]);
			total_freed++;
		}
	}
}

//Fill the heap, deallocate randomly half object
void task_4(void){
	char *obj[64];

	//Fill the heap
	for(int i = 0; i < 64; i++){
		obj[i] = malloc(56);
	}

	//Deallocate random half object 
	int total_freed = 0;
        while(total_freed < 32){
                int index = rand() % 64;

                if(obj[index]){
                        free(obj[index]);
                        obj[index] = NULL;
                        total_freed++;
                }
        }

	//Deallocate remaining objects
	for(int i = 0; i < 64; i++){
		if(obj[i]){
			free(obj[i]);
		}
	}
}

//Fill heap, deallocate first 10 objects and allocate back 10 object with smaller size
void task_5(void){
	char *obj[64];

	//Fill heap
	for(int i = 0; i < 64; i++){
		obj[i] = malloc(56);
	}

	//Deallocate first 10 objects
	for(int i = 0; i < 10; i++){
		free(obj[i]);
		obj[i] = NULL;
	}

	//Reallocate 10 objects with smaller size
	for(int i = 0; i < 10; i++){
		obj[i] = malloc(24);
	}

	//Deallocate all objects
	for(int i = 0; i < 64; i++){
		free(obj[i]);
	}
}

//Average runtime for each workload after execute 50 times
double run_time(void (*func)(void)){
	struct timeval start, end;
	double total_time = 0.0;

	for(int i = 0; i < RUNS; i++){
		gettimeofday(&start, NULL);
		func();
		gettimeofday(&end, NULL);
		
		long seconds = end.tv_sec - start.tv_sec;
		long microseconds = end.tv_usec - start.tv_usec;

		total_time += (double)seconds + ((double)microseconds) * 1e-6;
	}

	return total_time / RUNS;
}

int main(int argc, char **argv){
	srand(time(NULL));

	printf("Task 1 avg time: %.6f secs\n", run_time(task_1));
	printf("Task 2 avg time: %.6f secs\n", run_time(task_2));
	printf("Task 3 avg time: %.6f secs\n", run_time(task_3));
	printf("Task 4 avg time: %.6f secs\n", run_time(task_4));
	printf("Task 5 avg time: %.6f secs\n", run_time(task_5));

	return EXIT_SUCCESS;
}
