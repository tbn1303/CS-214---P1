#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
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
        int total_allocated = 0; //Count total allocated object
        int total_freed = 0; //Count total freed object (for debug purpose)

        while(total_allocated < ALLOCATES){
                int choice = rand() % 2;

                //Allocate object
                if(choice == 0){
                        for(int i = 0; i < ALLOCATES, i++){
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

                total_time += seconds + microseconds * 1e-6;
        }

        return total_time / RUNS;
}

int main(int argc, char **argv){
        srand(time(NULL));

        printf("Task A avg time: %.6f secs\n", run_time(task_1));
        printf("Task B avg time: %.6f secs\n", run_time(task_2));

        return EXIT_SUCCESS;
}
