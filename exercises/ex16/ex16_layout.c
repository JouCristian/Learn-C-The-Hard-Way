#include <stddef.h>
#include <stdio.h>

struct Person
{
    char *name;
    int age;
    int height;
    int weight;
};

int main(void)
{
    printf("sizeof(char *) = %zu\n",
           sizeof(char *));

    printf("sizeof(int) = %zu\n",
           sizeof(int));

    printf("sizeof(struct Person) = %zu\n",
           sizeof(struct Person));

    printf("offset of name = %zu\n",
           offsetof(struct Person, name));

    printf("offset of age = %zu\n",
           offsetof(struct Person, age));

    printf("offset of height = %zu\n",
           offsetof(struct Person, height));

    printf("offset of weight = %zu\n",
           offsetof(struct Person, weight));

    return 0;
}
