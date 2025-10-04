#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

#define RUN 50
#define ALLOCATES 120

//Allocate and deallocate immediately 1-byte object, 120 times
void task_A(){
        for(int i = 0; i < ALLOCATES; i++){
                char *p = malloc(1);
                free(p);
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

//
