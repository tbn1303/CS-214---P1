#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

void correctness_testing(){
	char *obj[12];

	//Allocate 12 object of 32 bytes and fill them up with distinct pattern
	for(int i = 0; i < 12; i++){
		obj[i] = malloc(32);
		
		//If allocation fail, stop testing
		if(!obj[i]){
			printf("Fail to allocate object %d, test is stopped.\n", i);
			return;
		}

		//Fill each object with distinct pattern
		memset(obj[i], i * 2, 32);
	}

	//Check for overlap
	for(int i = 0; i < 12; i++){
		for(int j = 0; j < 32; j++){
			if(obj[i][j] != (char)(i * 2)){
				printf("Memomry overlap detected at object %d, byte %d\n", i, j);
			}
		}
	}

	//Check for coalesce
	free(obj[1]); obj[1] = NULL;
	free(obj[2]); obj[2] = NULL;

	char *c = malloc(56);
	if(!c){
		printf("Coalesce failed\n");
	}

	else{
		free(c);
	}

	//Free all object and check for double free
	for(int i = 0; i < 12; i++){
		if(obj[i]){
			free(obj[i]);
			obj[i] = NULL;
		}
	}

	//Double free
	free(obj[0]);
	free(obj[1]);

	printf("Correctness testing is completed!\n");
}

int main(int argc, char **argv){
	correctness_testing();

	return EXIT_SUCCESS;
}
