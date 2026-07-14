#include <stdio.h>

int main(void)
{
    int ages[] = {23, 43, 12, 89, 2};

    /*
     * Deliberately wrong:
     * treat integer bytes as a pointer to a string.
     */
    const char **bad_name = (const char **)ages;

    printf("Bad name: %d\n",**bad_name);

    return 0;
}
