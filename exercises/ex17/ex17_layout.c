#include <stddef.h>
#include <stdio.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address
{
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database
{
    struct Address rows[MAX_ROWS];
};

struct Connection
{
    FILE *file;
    struct Database *db;
};

int main(void)
{
    printf("sizeof(int) = %zu\n",
           sizeof(int));

    printf("sizeof(struct Address) = %zu\n",
           sizeof(struct Address));

    printf("offset of id = %zu\n",
           offsetof(struct Address, id));

    printf("offset of set = %zu\n",
           offsetof(struct Address, set));

    printf("offset of name = %zu\n",
           offsetof(struct Address, name));

    printf("offset of email = %zu\n",
           offsetof(struct Address, email));

    printf("sizeof(struct Database) = %zu\n",
           sizeof(struct Database));

    printf("sizeof(struct Connection) = %zu\n",
           sizeof(struct Connection));

    return 0;
}
