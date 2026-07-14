#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int can_print_it(int ch)
{
    return isalpha(ch) || isblank(ch);
}

void print_letters(const char arg[], size_t length)
{
    size_t i = 0;

    for (i = 0; i < length; i++)
    {
        int ch = (unsigned char)arg[i];

        if (can_print_it(ch))
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
        size_t length = strlen(argv[i]);

        printf("argument %d has %zu characters: ",
               i, length);

        print_letters(argv[i], length);
    }
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
