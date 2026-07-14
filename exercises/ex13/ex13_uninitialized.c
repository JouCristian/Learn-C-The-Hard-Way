#include <stdio.h>

int main(void)
{
    char letter;

    switch (letter)
    {
        case 'A':
            printf("A\n");
            break;

        default:
            printf("Other\n");
            break;
    }

    return 0;
}
