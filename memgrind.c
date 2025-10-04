#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define RUNS 50
#define ALLOCATES 120

//Allocate and deallocate immediately 1-byte object, 120 times
void task_A(){
        for(int i = 0; i < ALLOCATES; i++){
                char *p = malloc(1);
                free(p);
        }
}

//Allocate of 120 1-byte objects, then deallocate them
void task_B(){
        char *obj[ALLOCATES];
        for(int i = 0; i < ALLOCATES; i++){
                obj[i] = malloc(1);
        }

        for(int i = 0; i < ALLOCATES; i++){
                free(obj[i]);
        }
}

//Average runtime for each workload after execute 50 times
double run_time(void (*func())){
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

        printf("Task A avg time: %.6f secs\n", run_time(Task_A));
        printf("Task B avg time: %.6f secs\n", run_time(Task_B));

        return EXIT_SUCCESS;
}
