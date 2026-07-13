#include<limits.h>
#include<stdio.h>

int main(void){
	printf("sizeof(int) = %zu bytes\n",sizeof(int));
	printf("sizeof(long) = %zu bytes\n",sizeof(long));
	printf("sizeof(unsigned long) = %zu bytes\n",sizeof(unsigned long));

	printf("LONG_MIN = %ld\n",LONG_MIN);
	printf("LONG_MAX = %ld\n",LONG_MAX);
	printf("ULONG_MAX = %lu\n",ULONG_MAX);

	unsigned long wrapped = ULONG_MAX + 1UL;

	printf("ULONG_MAX + 1 = %lu\n",wrapped);

	return 0;
}
