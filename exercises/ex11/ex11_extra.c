#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = argc;

    printf("Arguments in reverse:\n");

    while (i > 0)
    {
        i--;
        printf("arg %d: %s\n", i, argv[i]);
    }

    const char *states[] = {
        "California",
        "Oregon",
        "Washington",
        "Texas"
    };

    int num_states =
        (int)(sizeof(states) / sizeof(states[0]));

    i = num_states;

    printf("States in reverse:\n");

    while (i > 0)
    {
        i--;
        printf("state %d: %s\n", i, states[i]);
    }

        int arg_index = 1;
    int state_index = 0;

    printf("Copy argv pointers into states:\n");

    while (arg_index < argc &&
           state_index < num_states)
    {
        states[state_index] = argv[arg_index];

        printf("states[%d] = %s\n",
               state_index, states[state_index]);

        printf("states address: %p, argv address: %p\n",
               (void *)states[state_index],
               (void *)argv[arg_index]);

        arg_index++;
        state_index++;
    }

    printf("States after assignment:\n");

    i = 0;

    while (i < num_states)
    {
        printf("state %d: %s\n", i, states[i]);
        i++;
    }

    return 0;
}
