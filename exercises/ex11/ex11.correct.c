#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    /* Go through each string in argv. */
    while (i < argc)
    {
        printf("arg %d: %s\n", i, argv[i]);
        i++;
    }

    /* Make our own array of strings. */
    const char *states[] = {
        "California",
        "Oregon",
        "Washington",
        "Texas"
    };

    int num_states = 4;

    /*
     * The first loop ended with i == argc.
     * Reset i before using it for another array.
     */
    i = 0;

    while (i < num_states)
    {
        printf("state %d: %s\n", i, states[i]);
        i++;
    }

    return 0;
}
