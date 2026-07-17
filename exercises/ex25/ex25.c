/*
 * isspace函数的声明位于ctype.h中。
 * 它用于判断空格、Tab等空白字符。
 */
#include <ctype.h>

/*
 * errno和ERANGE的声明位于errno.h中。
 * strtol转换失败或越界时可能设置errno。
 */
#include <errno.h>

/*
 * INT_MIN和INT_MAX声明在limits.h中。
 * 它们表示int能够保存的最小值和最大值。
 */
#include <limits.h>

/*
 * va_list、va_start、va_arg和va_end
 * 都声明在stdarg.h中。
 */
#include <stdarg.h>

/*
 * printf、fprintf、fgets、fgetc、stdin和stdout
 * 都声明在stdio.h中。
 */
#include <stdio.h>

/*
 * calloc、free、strtol以及程序退出状态
 * 声明在stdlib.h中。
 */
#include <stdlib.h>

/*
 * strchr函数声明在string.h中。
 * 它用于寻找字符串中的换行符。
 */
#include <string.h>

/*
 * 引入练习20编写的错误检查宏。
 */
#include "dbg.h"

/*
 * 定义本程序允许读取的最大文本容量。
 *
 * 数组中的最后一个字节需要留给'\0'，
 * 因此最多保存99个普通字符。
 */
#define MAX_DATA 100

/*
 * 清理当前输入行中剩余的字符。
 *
 * 当用户输入内容过长时，
 * 需要把这一行剩余内容全部读掉，
 * 防止它们影响下一次输入。
 */
static void discard_rest_of_line(void)
{
    /*
     * ch必须使用int类型，
     * 因为fgetc除了返回字符，还可能返回EOF。
     */
    int ch = 0;

    /*
     * 不断读取字符，
     * 直到遇到换行符或者文件结束。
     */
    while ((ch = fgetc(stdin)) != '\n' &&
           ch != EOF)
    {
        /*
         * 循环体不需要处理这些字符，
         * 只负责将它们从输入流中清除。
         */
    }
}

/*
 * 从标准输入读取一行字符串。
 *
 * out_string是char **，也就是指向char *的指针。
 *
 * 函数会在堆上创建字符串，
 * 再通过out_string把字符串地址交给调用者。
 *
 * max_buffer表示字符串缓冲区的总容量，
 * 其中包括最后的'\0'。
 *
 * 成功返回0，失败返回-1。
 */
static int read_string(char **out_string,
                       size_t max_buffer)
{
    /*
     * result用于保存fgets的返回值。
     */
    char *result = NULL;

    /*
     * newline用于保存换行符所在的地址。
     */
    char *newline = NULL;

    /*
     * out_string必须是有效的二级指针。
     */
    check(out_string != NULL,
          "Output string pointer is NULL.");

    /*
     * 至少需要两个字节：
     *
     * 一个保存普通字符；
     * 一个保存字符串结束符'\0'。
     */
    check(max_buffer >= 2,
          "String buffer is too small.");

    /*
     * 先把调用者的字符串指针设为NULL。
     *
     * 如果后续发生错误，
     * 调用者不会得到未初始化的垃圾地址。
     */
    *out_string = NULL;

    /*
     * calloc申请max_buffer字节的堆内存，
     * 并把所有字节初始化为0。
     */
    *out_string =
        calloc(max_buffer, sizeof(char));

    /*
     * 检查内存申请是否成功。
     */
    check_mem(*out_string);

    /*
     * fgets最多读取max_buffer - 1个字符，
     * 并自动在末尾添加'\0'。
     */
    result = fgets(*out_string,
                   (int)max_buffer,
                   stdin);

    /*
     * fgets返回NULL表示读取失败或遇到文件结束。
     */
    check(result != NULL,
          "Failed to read string.");

    /*
     * 在读到的字符串中寻找换行符。
     */
    newline = strchr(*out_string, '\n');

    /*
     * 找到换行符时，
     * 用'\0'覆盖它，从字符串中删除换行。
     */
    if (newline != NULL)
    {
        /*
         * 将换行符替换成字符串结束符。
         */
        *newline = '\0';
    }
    else
    {
        /*
         * 没找到换行符可能有两种情况：
         *
         * 1. 输入内容正好占满缓冲区；
         * 2. 输入内容超过了缓冲区。
         *
         * 读取下一个字符来判断。
         */
        int next = fgetc(stdin);

        /*
         * 如果下一个字符不是换行符，
         * 也不是EOF，说明输入确实过长。
         */
        if (next != '\n' && next != EOF)
        {
            /*
             * 清理这一行剩余内容。
             */
            discard_rest_of_line();

            /*
             * 报告输入过长并跳转到error。
             */
            sentinel("Input string is too long.");
        }
    }

    /*
     * 返回0表示读取成功。
     */
    return 0;

/*
 * check、check_mem或sentinel失败时，
 * 都会通过goto跳到这里。
 */
error:
    /*
     * 只有out_string本身有效时，
     * 才能访问*out_string。
     */
    if (out_string != NULL)
    {
        /*
         * free(NULL)也是安全的。
         *
         * 如果前面已经成功申请内存，
         * 这里会将其释放。
         */
        free(*out_string);

        /*
         * 将调用者的指针重新设置为NULL，
         * 避免保留悬空指针。
         */
        *out_string = NULL;
    }

    /*
     * 返回-1表示读取失败。
     */
    return -1;
}

/*
 * 从标准输入读取并转换一个整数。
 *
 * out_int是输出参数。
 *
 * 成功时函数会通过*out_int保存结果。
 *
 * 成功返回0，失败返回-1。
 */
static int read_int(int *out_int)
{
    /*
     * input用于保存read_string创建的字符串。
     */
    char *input = NULL;

    /*
     * end用于保存strtol停止转换的位置。
     */
    char *end = NULL;

    /*
     * value先使用long保存转换结果，
     * 方便检查它是否超过int范围。
     */
    long value = 0;

    /*
     * out_int必须是有效指针。
     */
    check(out_int != NULL,
          "Output integer pointer is NULL.");

    /*
     * 读取一整行文本。
     */
    int rc = read_string(&input, MAX_DATA);

    /*
     * read_string返回0才表示成功。
     */
    check(rc == 0,
          "Failed to read integer input.");

    /*
     * 清除之前遗留的errno。
     */
    errno = 0;

    /*
     * 使用十进制方式将字符串转换为long。
     *
     * end会指向没有参与转换的第一个字符。
     */
    value = strtol(input, &end, 10);

    /*
     * end == input表示一个数字都没有读到。
     */
    check(end != input,
          "Input is not an integer.");

    /*
     * 跳过数字后允许存在的空白字符。
     */
    while (*end != '\0' &&
           isspace((unsigned char)*end))
    {
        /*
         * 移动到下一个字符。
         */
        end++;
    }

    /*
     * 跳过空白后必须到达字符串末尾。
     *
     * 例如"12abc"会在这里被拒绝。
     */
    check(*end == '\0',
          "Integer contains invalid characters.");

    /*
     * ERANGE表示数值超出了long的范围。
     */
    check(errno != ERANGE,
          "Integer is outside the supported range.");

    /*
     * 继续检查结果是否适合存入int。
     */
    check(value >= INT_MIN &&
          value <= INT_MAX,
          "Integer does not fit in int.");

    /*
     * 所有检查通过后，
     * 把结果转换为int并写入输出参数。
     */
    *out_int = (int)value;

    /*
     * 输入字符串已经使用完毕，
     * 释放read_string申请的内存。
     */
    free(input);

    /*
     * 防止继续误用已经释放的地址。
     */
    input = NULL;

    /*
     * 返回0表示成功。
     */
    return 0;

/*
 * 任意check失败时跳转到这里。
 */
error:
    /*
     * 释放可能已经申请的字符串。
     */
    free(input);

    /*
     * 返回-1表示读取整数失败。
     */
    return -1;
}

/*
 * 从标准输入读取一个字符。
 *
 * 本函数要求用户一行中只输入一个字符。
 *
 * 成功返回0，失败返回-1。
 */
static int read_char(char *out_char)
{
    /*
     * input用于保存用户输入的一整行。
     */
    char *input = NULL;

    /*
     * 输出字符指针必须有效。
     */
    check(out_char != NULL,
          "Output character pointer is NULL.");

    /*
     * 读取一整行文本。
     */
    int rc = read_string(&input, MAX_DATA);

    /*
     * 检查字符串读取是否成功。
     */
    check(rc == 0,
          "Failed to read character.");

    /*
     * input[0]不能是'\0'，否则表示输入为空。
     *
     * input[1]必须是'\0'，
     * 保证用户只输入了一个字符。
     */
    check(input[0] != '\0' &&
          input[1] == '\0',
          "Enter exactly one character.");

    /*
     * 把第一个字符保存到输出参数。
     */
    *out_char = input[0];

    /*
     * 释放临时字符串。
     */
    free(input);

    /*
     * 防止继续使用悬空地址。
     */
    input = NULL;

    /*
     * 返回0表示成功。
     */
    return 0;

/*
 * 任意check失败时跳转到这里。
 */
error:
    /*
     * 释放可能已经申请的字符串。
     */
    free(input);

    /*
     * 返回-1表示失败。
     */
    return -1;
}

/*
 * read_scan是一个变参函数。
 *
 * fmt是固定参数。
 *
 * ...表示fmt后面可以继续传入任意数量的参数。
 *
 * 本程序支持：
 *
 * %d  读取一个int
 * %c  读取一个char
 * %s  读取一个动态字符串
 *
 * 使用%s时，需要额外传入：
 *
 * 缓冲区容量
 * char **输出参数
 */
static int read_scan(const char *fmt, ...)
{
    /*
     * i用于遍历格式字符串。
     */
    size_t i = 0;

    /*
     * rc用于保存各个读取函数的返回值。
     */
    int rc = 0;

    /*
     * argp用于访问...中的可变参数。
     */
    va_list argp;

    /*
     * 格式字符串必须有效。
     */
    check(fmt != NULL,
          "Format string is NULL.");

    /*
     * va_start初始化argp。
     *
     * 第二个参数必须是变参函数中
     * 最后一个固定参数，也就是fmt。
     */
    va_start(argp, fmt);

    /*
     * 逐字符遍历格式字符串。
     */
    for (i = 0; fmt[i] != '\0'; i++)
    {
        /*
         * 普通字符不是格式占位符。
         */
        if (fmt[i] != '%')
        {
            /*
             * 为了保持这个简化版接口清晰，
             * 普通空白字符会被直接忽略。
             */
            if (isspace((unsigned char)fmt[i]))
            {
                continue;
            }

            /*
             * 其他普通字符暂不支持。
             */
            sentinel("Unsupported literal in format.");
        }

        /*
         * 当前字符是'%'，
         * 移动到它后面的格式字符。
         */
        i++;

        /*
         * 如果'%'正好位于格式字符串末尾，
         * 说明格式不完整。
         */
        if (fmt[i] == '\0')
        {
            sentinel("Format string ends with %%.");
        }

        /*
         * 根据格式字符读取不同类型的参数。
         */
        switch (fmt[i])
        {
            /*
             * %d表示读取int。
             */
            case 'd':
            {
                /*
                 * 从可变参数列表中取得int *。
                 *
                 * 调用者必须实际传入int *，
                 * 否则行为未定义。
                 */
                int *out_int =
                    va_arg(argp, int *);

                /*
                 * 调用read_int读取整数。
                 */
                rc = read_int(out_int);

                /*
                 * 检查读取是否成功。
                 */
                check(rc == 0,
                      "Failed to read int.");

                /*
                 * 当前case结束。
                 */
                break;
            }

            /*
             * %c表示读取char。
             */
            case 'c':
            {
                /*
                 * 从变参列表中取得char *。
                 */
                char *out_char =
                    va_arg(argp, char *);

                /*
                 * 读取一个字符。
                 */
                rc = read_char(out_char);

                /*
                 * 检查读取是否成功。
                 */
                check(rc == 0,
                      "Failed to read char.");

                /*
                 * 当前case结束。
                 */
                break;
            }

            /*
             * %s表示读取动态字符串。
             */
            case 's':
            {
                /*
                 * 可变参数中的整数会经过默认参数提升，
                 * 因此MAX_DATA在这里按照int读取。
                 */
                int max_buffer =
                    va_arg(argp, int);

                /*
                 * 下一个参数必须是char **。
                 *
                 * read_string会通过它修改调用者的char *。
                 */
                char **out_string =
                    va_arg(argp, char **);

                /*
                 * 字符串容量必须合理。
                 */
                check(max_buffer >= 2,
                      "Invalid string buffer size.");

                /*
                 * 调用read_string创建并读取字符串。
                 */
                rc = read_string(
                    out_string,
                    (size_t)max_buffer
                );

                /*
                 * 检查字符串是否读取成功。
                 */
                check(rc == 0,
                      "Failed to read string.");

                /*
                 * 当前case结束。
                 */
                break;
            }

            /*
             * %%表示格式中的一个百分号。
             *
             * 当前程序不需要从输入读取百分号，
             * 因此这里只允许它存在，不执行操作。
             */
            case '%':
            {
                /*
                 * 不需要读取额外参数。
                 */
                break;
            }

            /*
             * 处理所有未知格式。
             */
            default:
            {
                /*
                 * 报告非法格式并跳转到error。
                 */
                sentinel("Unsupported format: %%%c.",
                         fmt[i]);
            }
        }
    }

    /*
     * 每次va_start之后必须调用va_end。
     */
    va_end(argp);

    /*
     * 返回0表示全部读取成功。
     */
    return 0;

/*
 * check或sentinel失败时跳转到这里。
 */
error:
    /*
     * 只有执行过va_start后才能调用va_end。
     *
     * 当前程序中进入循环前已经执行了va_start。
     */
    va_end(argp);

    /*
     * 返回-1表示读取失败。
     */
    return -1;
}

/*
 * 程序入口。
 */
int main(int argc, char *argv[])
{
    /*
     * 当前程序不使用命令行参数。
     *
     * 转换为void可以消除编译器的未使用参数警告。
     */
    (void)argc;
    (void)argv;

    /*
     * first_name是字符串指针。
     *
     * read_scan中的%s会申请内存，
     * 再把地址保存到这里。
     */
    char *first_name = NULL;

    /*
     * initial用于保存用户名字的首字母。
     */
    char initial = '\0';

    /*
     * last_name同样由read_scan动态创建。
     */
    char *last_name = NULL;

    /*
     * age用于保存用户年龄。
     */
    int age = 0;

    /*
     * rc用于保存read_scan返回值。
     */
    int rc = 0;

    /*
     * 打印名字输入提示。
     */
    printf("What's your first name? ");

    /*
     * 提示没有换行，
     * 所以主动刷新stdout。
     */
    fflush(stdout);

    /*
     * %s需要传入两个可变参数：
     *
     * MAX_DATA      缓冲区容量
     * &first_name   char **输出参数
     */
    rc = read_scan("%s",
                   MAX_DATA,
                   &first_name);

    /*
     * 检查名字是否读取成功。
     */
    check(rc == 0,
          "Failed to read first name.");

    /*
     * 不允许名字为空。
     */
    check(first_name[0] != '\0',
          "First name cannot be empty.");

    /*
     * 打印首字母输入提示。
     */
    printf("What's your initial? ");

    /*
     * 立即显示提示。
     */
    fflush(stdout);

    /*
     * %c需要传入char *。
     */
    rc = read_scan("%c",
                   &initial);

    /*
     * 检查字符是否读取成功。
     */
    check(rc == 0,
          "Failed to read initial.");

    /*
     * 打印姓氏输入提示。
     */
    printf("What's your last name? ");

    /*
     * 立即显示提示。
     */
    fflush(stdout);

    /*
     * 读取姓氏。
     */
    rc = read_scan("%s",
                   MAX_DATA,
                   &last_name);

    /*
     * 检查姓氏是否读取成功。
     */
    check(rc == 0,
          "Failed to read last name.");

    /*
     * 不允许姓氏为空。
     */
    check(last_name[0] != '\0',
          "Last name cannot be empty.");

    /*
     * 打印年龄提示。
     */
    printf("How old are you? ");

    /*
     * 立即显示提示。
     */
    fflush(stdout);

    /*
     * %d需要传入int *。
     */
    rc = read_scan("%d",
                   &age);

    /*
     * 检查年龄是否读取成功。
     */
    check(rc == 0,
          "Failed to read age.");

    /*
     * 做一个基本年龄范围检查。
     */
    check(age >= 0 && age <= 150,
          "Age must be between 0 and 150.");

    /*
     * 打印结果标题。
     */
    printf("\n---- RESULTS ----\n");

    /*
     * 输出名字。
     *
     * read_string已经删除了换行符，
     * 因此这里手动添加换行。
     */
    printf("First Name: %s\n",
           first_name);

    /*
     * 输出首字母。
     */
    printf("Initial: '%c'\n",
           initial);

    /*
     * 输出姓氏。
     */
    printf("Last Name: %s\n",
           last_name);

    /*
     * 输出年龄。
     */
    printf("Age: %d\n",
           age);

    /*
     * first_name由calloc创建，
     * 使用完毕后必须free。
     */
    free(first_name);

    /*
     * 清空指针，防止误用悬空地址。
     */
    first_name = NULL;

    /*
     * last_name同样需要释放。
     */
    free(last_name);

    /*
     * 清空指针。
     */
    last_name = NULL;

    /*
     * 返回成功状态。
     */
    return EXIT_SUCCESS;

/*
 * 任意check失败后跳转到这里。
 */
error:
    /*
     * 无论错误发生在哪一步，
     * 都要释放已经成功创建的名字字符串。
     *
     * free(NULL)是安全的。
     */
    free(first_name);

    /*
     * 释放姓氏字符串。
     */
    free(last_name);

    /*
     * 返回失败状态。
     */
    return EXIT_FAILURE;
}
