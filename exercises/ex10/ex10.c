#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 0;

    /* Go through each string in argv.
     * argv[0] is skipped because it is normally the program name.
     */
    for (i = 1; i < argc; i++)
    {
        printf("arg %d: %s\n", i, argv[i]);
    }

    /* Make our own array of strings. */
    char *states[] = {
        "California",
        "NewYork",
        "Washington",
        "Texas"
    };

    int num_states = 4;

    for (i = 0; i < num_states; i++)
    {
	    if(states[i] != NULL)
	    {
	    	printf("state %d: %s\n",i,states[i]);
	    }
	    else
	    {
	    	printf("state %d: (null)\n",i);
	    }
    }

    return 0;
}
