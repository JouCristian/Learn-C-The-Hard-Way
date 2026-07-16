#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * compare_cb是一个函数指针类型。
 *
 * 它可以指向这种函数：
 *
 *     int function(int a, int b);
 */
typedef int (*compare_cb)(int a, int b);

static void die(const char *message)
{
    fprintf(stderr, "ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

/*
 * 安全地把字符串转换为int。
 *
 * 成功返回1，失败返回0。
 */
static int parse_int(const char *text, int *result)
{
    char *end = NULL;

    errno = 0;

    long value = strtol(text, &end, 10);

    if (errno == ERANGE)
    {
        return 0;
    }

    /*
     * end == text：
     * 一个数字都没有读取成功。
     *
     * *end != '\0'：
     * 数字后面存在非法字符。
     */
    if (end == text || *end != '\0')
    {
        return 0;
    }

    if (value < INT_MIN || value > INT_MAX)
    {
        return 0;
    }

    *result = (int)value;

    return 1;
}

/*
 * 使用比较回调的冒泡排序。
 *
 * numbers不会被直接修改。
 * 函数会创建并返回一个新的已排序数组。
 */
static int *bubble_sort(const int *numbers,
                        size_t count,
                        compare_cb cmp)
{
    size_t i = 0;
    size_t j = 0;

    if (numbers == NULL)
    {
        die("Input array is NULL");
    }

    if (cmp == NULL)
    {
        die("Comparison callback is NULL");
    }

    if (count == 0)
    {
        die("Cannot sort an empty array");
    }

    /*
     * 防止count * sizeof(*target)发生无符号溢出。
     */
    if (count > SIZE_MAX / sizeof(int))
    {
        die("Array is too large");
    }

    int *target = malloc(count * sizeof(*target));

    if (target == NULL)
    {
        die("Cannot allocate sorting buffer");
    }

    /*
     * int数组中不包含内部指针，
     * 因此可以直接复制它的原始字节。
     */
    memcpy(target,
           numbers,
           count * sizeof(*target));

    for (i = 0; i < count; i++)
    {
        for (j = 0; j + 1 < count; j++)
        {
            /*
             * cmp返回正数，表示左边应该放到右边后面。
             */
            if (cmp(target[j], target[j + 1]) > 0)
            {
                int temp = target[j];

                target[j] = target[j + 1];
                target[j + 1] = temp;
            }
        }
    }

    return target;
}

/*
 * 升序比较。
 *
 * 不直接写return a - b，
 * 因为极端整数相减可能溢出。
 */
static int sorted_order(int a, int b)
{
    return (a > b) - (a < b);
}

/*
 * 降序比较。
 */
static int reverse_order(int a, int b)
{
    return (a < b) - (a > b);
}

/*
 * 故意设计的奇怪比较方式。
 *
 * 它不满足可靠排序关系所需的全部规则，
 * 所以输出不能理解为真正有意义的顺序。
 */
static int strange_order(int a, int b)
{
    if (a == 0 || b == 0)
    {
        return 0;
    }

    return a % b;
}

static void test_sorting(const char *label,
                         const int *numbers,
                         size_t count,
                         compare_cb cmp)
{
    size_t i = 0;

    int *sorted =
        bubble_sort(numbers, count, cmp);

    printf("%s: ", label);

    for (i = 0; i < count; i++)
    {
        printf("%d ", sorted[i]);
    }

    printf("\n");

    free(sorted);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        die("USAGE: ex18 4 3 1 5 6");
    }

    size_t count = (size_t)(argc - 1);

    /*
     * 跳过argv[0]中的程序名称。
     */
    char **inputs = argv + 1;

    if (count > SIZE_MAX / sizeof(int))
    {
        die("Too many input numbers");
    }

    int *numbers =
        malloc(count * sizeof(*numbers));

    if (numbers == NULL)
    {
        die("Cannot allocate input array");
    }

    size_t i = 0;

    for (i = 0; i < count; i++)
    {
        if (!parse_int(inputs[i], &numbers[i]))
        {
            fprintf(stderr,
                    "ERROR: invalid integer: %s\n",
                    inputs[i]);

            free(numbers);

            return EXIT_FAILURE;
        }
    }

    test_sorting("sorted",
                 numbers,
                 count,
                 sorted_order);

    test_sorting("reverse",
                 numbers,
                 count,
                 reverse_order);

    test_sorting("strange",
                 numbers,
                 count,
                 strange_order);

    free(numbers);

    return EXIT_SUCCESS;
}
