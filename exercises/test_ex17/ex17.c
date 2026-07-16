#include<stdio.h>
#include<errno.h>

int main(int argc, char *argv[]){
	FILE *file = fopen("does_not_exist.txt","r");

	if(file == NULL){
		fprintf(stderr, "ERROR: %s\n",argv[1]);
		perror(argv[1]);
	}
	return 0;
}
