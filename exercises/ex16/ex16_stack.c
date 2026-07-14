#include <stdio.h>

struct Person
{
    char name[32];
    int age;
    int height;
    int weight;
};

/*
 * The whole structure is passed by value.
 */
void Person_print(struct Person who)
{
    printf("Name: %s\n", who.name);
    printf("\tAge: %d\n", who.age);
    printf("\tHeight: %d\n", who.height);
    printf("\tWeight: %d\n", who.weight);
}

int main(void)
{
    /*
     * These structures are local variables stored on the stack.
     */
    struct Person joe = {
        "Joe Alex",
        32,
        64,
        140
    };

    struct Person frank = {
        "Frank Blank",
        20,
        72,
        180
    };

    /*
     * joe is an object, not a pointer, so use the dot operator.
     */
    joe.age += 20;
    joe.height -= 2;
    joe.weight += 40;

    frank.age += 20;
    frank.weight += 20;

    Person_print(joe);
    Person_print(frank);

    /*
     * No free is needed. Local variables are destroyed
     * automatically when main returns.
     */
    return 0;
}
