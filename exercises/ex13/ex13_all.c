#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("ERROR: You need at least one argument.\n");
        return 1;
    }

    int arg_index;
    int char_index;

    for (arg_index = 1; arg_index < argc; arg_index++)
    {
        printf("Argument %d: %s\n",
               arg_index, argv[arg_index]);

        for (char_index = 0;
             argv[arg_index][char_index] != '\0';
             char_index++)
        {
            char letter = argv[arg_index][char_index];

            switch (letter)
            {
                case 'a':
                case 'A':
                    printf("  %d: 'A'\n", char_index);
                    break;

                case 'e':
                case 'E':
                    printf("  %d: 'E'\n", char_index);
                    break;

                case 'i':
                case 'I':
                    printf("  %d: 'I'\n", char_index);
                    break;

                case 'o':
                case 'O':
                    printf("  %d: 'O'\n", char_index);
                    break;

                case 'u':
                case 'U':
                    printf("  %d: 'U'\n", char_index);
                    break;

                default:
                    printf("  %d: %c is not a vowel\n",
                           char_index, letter);
                    break;
            }
        }
    }

    return 0;
}
