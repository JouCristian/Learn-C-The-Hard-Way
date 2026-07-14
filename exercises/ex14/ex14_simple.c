#include <ctype.h>
#include <stdio.h>

void print_letters(const char arg[])
{
    int i = 0;

    for (i = 0; arg[i] != '\0'; i++)
    {
        int ch = (unsigned char)arg[i];

        if (isalpha(ch) || isblank(ch))
        {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

void print_arguments(int argc, char *argv[])
{
    int i = 0;

    for (i = 0; i < argc; i++)
    {
        print_letters(argv[i]);
    }
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
