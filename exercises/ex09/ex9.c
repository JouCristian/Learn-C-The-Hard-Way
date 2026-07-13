#include <stdio.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    int numbers[4] = {0};
    char name[4] = {'a'};

    /* First, print them out raw. */
    printf("numbers: %d %d %d %d\n",
           numbers[0], numbers[1],
           numbers[2], numbers[3]);

    printf("name each: %c %c %c %c\n",
           name[0], name[1],
           name[2], name[3]);

    printf("name: %s\n", name);

    /* Set up the numbers. */
    numbers[0] = 'Z';
    numbers[1] = 'e';
    numbers[2] = 'd';
    numbers[3] = '\0';

    /* Set up the name. */
    name[0] = 'Z';
    name[1] = 'e';
    name[2] = 'd';
    name[3] = '\0';

    printf("numbers: %d %d %d %d\n",
           numbers[0], numbers[1],
           numbers[2], numbers[3]);

    printf("name each: %d %d %d %d\n",
           name[0], name[1],
           name[2], name[3]);

    fwrite(name,sizeof(char),7,stdout);
    putchar('\n');

    printf("name: %s\n", name);

    /* Another way to refer to a string literal. */
    const char *another = "Zed";

    printf("another: %s\n", another);

    printf("another each: %c %c %c %c\n",
           another[0], another[1],
           another[2], another[3]);

    return 0;
}
