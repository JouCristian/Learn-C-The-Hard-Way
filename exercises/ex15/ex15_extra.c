#include <stddef.h>
#include <stdio.h>

void print_people(const int *ages,
                  const char *const *names,
                  size_t count)
{
    size_t i = 0;

    for (i = 0; i < count; i++)
    {
        printf("%s is %d years old.\n",
               names[i], ages[i]);
    }
}

int main(int argc, char *argv[])
{
    int ages[] = {23, 43, 12, 89, 2};

    const char *names[] = {
        "Alan",
        "Frank",
        "Mary",
        "John",
        "Lisa"
    };

    size_t count = sizeof(ages) / sizeof(ages[0]);
    size_t i = 0;

    print_people(ages, names, count);

    printf("--- addresses ---\n");

    for (i = 0; i < count; i++)
    {
        printf("ages[%zu]: address=%p value=%d\n",
               i,
               (void *)&ages[i],
               ages[i]);

        printf("names[%zu]: pointer-slot=%p string=%p value=%s\n",
               i,
               (void *)&names[i],
               (void *)names[i],
               names[i]);
    }

    printf("--- reverse using pointers ---\n");

    int *age_ptr = ages + count;
    const char **name_ptr = names + count;

    while (age_ptr != ages)
    {
        age_ptr--;
        name_ptr--;

        printf("%s is %d years old.\n",
               *name_ptr,
               *age_ptr);
    }

    printf("--- command-line arguments using pointers ---\n");

    char **current_arg = argv;
    char **end_arg = argv + argc;

    while (current_arg < end_arg)
    {
        printf("argument address=%p, string=%s\n",
               (void *)*current_arg,
               *current_arg);

        current_arg++;
    }

    return 0;
}
