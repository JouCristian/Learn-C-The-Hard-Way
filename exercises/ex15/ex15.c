#include<stdio.h>

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	int ages[] = {23,43,12,89,2};

	const char *name[] = {
		"Alan",
		"Jou",
		"Mary",
		"Jack",
		"Lisa"
	};

	size_t count = sizeof(ages) / sizeof(ages[0]);
	size_t i = 0;

	/* First way: array indexing */
	for(i = 0; i < count; i++){
		printf("%s has %d years alive.\n",name[i],ages[i]);
	}

	printf("---\n");

	/* Point to the first elements of the arrays. */
	int *cur_age = ages;
	const char **cur_name = name;

	/* Second way: pointer arithmetic and dereferencing. */
	for(i = 0; i < count; i++){
		printf("%s is %d years old.\n",*(cur_name + i),*(cur_age + i));
	}

	printf("---\n");

	/* Third way: use array syntax on pointers. */
	for(i = 0; i < count; i++){
		printf("%s is %d years old again.\n",cur_name[i],cur_age[i]);
	}

	printf("---\n");

	/*
        * Fourth way: move the pointers themselves.
        * ages + count points one position past the array.
        */
	for(cur_name = name,cur_age = ages; cur_age < ages + count; cur_name++,cur_age++){
		printf("%s lived %d years so far.\n",*cur_name,*cur_age);
	}
	
	/*Let's see the differents between Array and Pointer*/
	printf("sizeof(ages) = %zu\n", sizeof(ages));
	printf("sizeof(cur_age) = %zu\n", sizeof(cur_age));

	printf("sizeof(name) = %zu\n", sizeof(name));
	printf("sizeof(cur_name) = %zu\n", sizeof(cur_name));
	
	return 0;
}
