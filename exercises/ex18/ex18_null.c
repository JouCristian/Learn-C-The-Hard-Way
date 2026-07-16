#include <stdio.h>

typedef int (*compare_cb)(int a, int b);

int main(void)
{
    compare_cb cmp = NULL;

    /*
     * 故意通过空函数指针调用。
     */
    printf("%d\n", cmp(1, 2));

    return 0;
}
