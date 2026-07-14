#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Person{
	char *name;
	int age;
	int height;
	int weight;
};

/*
 * Allocate and initialize a Person structure.
 *
 * name is copied, so the Person owns its own string.
 */
struct Person *Person_create(const char *name,int age,int height,int weight){
	struct Person *who = malloc(sizeof(*who));
	assert(who != NULL);

	who->name = strdup(name);
	assert(who->name != NULL);

	who->age = age;
    	who->height = height;
    	who->weight = weight;

	return who;
}

void Person_destroy(struct Person *who){
	assert(who != NULL);

	free(who->name);
	free(who);
}

void Person_print(const struct Person *who){
	printf("Name: %s\n",who->name);
	printf("\tAge: %d\n",who->age);
	printf("\tHeight: %d\n",who->height);
	printf("\tWeight: %d\n",who->weight);
}
int main(int argc, char *argv[]){
	(void)argc;
	(void)argv;
	struct Person *Joe = Person_create("Joe Alex",32,64,140);
	struct Person *Frank = Person_create("Frank Blank",20,72,180);

	printf("Joe is at memory location %p:\n",(void *)Joe);
	printf("Frank is at memory location %p:\n",(void *)Frank);

	//Modify structure members through pointers.
	Joe->age += 20;
	Joe->height -=2;
	Joe->weight +=40;

	printf("--- Joe after changes ---\n");
	Person_print(Joe);

	Frank->age += 20;
	Frank->weight += 20;

	printf("--- Frank after changes ---\n");
	Person_print(Frank);

	//Release the dynamically allocated memory.
	Person_destroy(Joe);
	Person_destroy(Frank);

	Joe = NULL;
	Frank = NULL;

	return 0;
}
