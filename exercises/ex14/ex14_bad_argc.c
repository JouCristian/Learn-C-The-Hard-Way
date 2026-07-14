#include <ctype.h>
#include <stdio.h>

/* Forward declarations. */
int can_print_it(int ch);
void print_letters(const char arg[]);

void print_arguments(int argc + 1, char *argv[])
{
    int i = 0;

    for (i = 0; i < argc; i++)
    {
        print_letters(argv[i]);
    }
}

void print_letters(const char arg[])
{
    int i = 0;

    for (i = 0; arg[i] != '\0'; i++)
    {
        /*
         * ctype函数要求参数是EOF，或者能够表示为
         * unsigned char的值。
         */
        int ch = (unsigned char)arg[i];

        if (can_print_it(ch))
        {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

int can_print_it(int ch)
{
    return isalpha(ch) || isblank(ch);
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
