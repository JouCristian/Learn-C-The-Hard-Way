
/*
 * 引入我们刚刚编写的调试宏。
 */
#include "dbg.h"

/*
 * FILE、fopen、fclose 的声明。
 */
#include <stdio.h>

/*
 * malloc 和 free 的声明。
 */
#include <stdlib.h>

/*
 * 测试 debug 调试宏。
 */
static void test_debug(void)
{
    /*
     * 不需要手动添加换行符，
     * debug 宏内部已经添加了 "\n"。
     */
    debug("I have brown hair.");

    /*
     * debug 支持和 printf 类似的格式化参数。
     */
    debug("I am %d years old.", 37);
}

/*
 * 测试错误日志宏。
 */
static void test_log_err(void)
{
    /*
     * 输出一条普通错误日志。
     */
    log_err("I believe everything is broken.");

    /*
     * 输出带有两个格式化参数的错误日志。
     */
    log_err("There are %d problems in %s.",
            0,
            "space");
}

/*
 * 测试警告日志宏。
 */
static void test_log_warn(void)
{
    /*
     * 输出一条警告。
     */
    log_warn("You can safely ignore this.");

    /*
     * 输出一条带字符串参数的警告。
     */
    log_warn("Maybe consider looking at: %s.",
             "/etc/passwd");
}

/*
 * 测试普通信息日志宏。
 */
static void test_log_info(void)
{
    /*
     * 输出普通程序信息。
     */
    log_info("Well, I did something mundane.");

    /*
     * %f 用于打印浮点数。
     */
    log_info("It happened %f times today.",
             1.3);
}

/*
 * 测试 check 和 check_mem。
 *
 * file_name 是需要尝试打开的文件名称。
 *
 * 成功返回 0，失败返回 -1。
 */
static int test_check(const char *file_name)
{
    /*
     * input 用于保存打开的文件指针。
     *
     * 初始设置为 NULL，
     * 便于 error 区域安全判断是否需要关闭。
     */
    FILE *input = NULL;

    /*
     * block 用于保存动态内存地址。
     *
     * 初始设置为 NULL，
     * 便于 error 区域统一释放。
     */
    char *block = NULL;

    /*
     * 申请 100 字节动态内存。
     */
    block = malloc(100);

    /*
     * 检查 malloc 是否成功。
     *
     * 如果 block 为 NULL，
     * 会跳转到下面的 error 标签。
     */
    check_mem(block);

    /*
     * 尝试以只读方式打开文件。
     *
     * 打开失败时 fopen 返回 NULL，
     * 并通常设置 errno。
     */
    input = fopen(file_name, "r");

    /*
     * 检查文件是否成功打开。
     *
     * 失败时日志中会显示：
     *
     * 文件名
     * 行号
     * 函数名
     * errno 对应的信息
     */
    check(input != NULL,
          "Failed to open %s.",
          file_name);

    /*
     * 文件已经成功打开，
     * 现在释放此前申请的动态内存。
     */
    free(block);

    /*
     * 将 block 设置为 NULL，
     * 防止以后误用已经释放的地址。
     */
    block = NULL;

    /*
     * 关闭文件。
     *
     * fclose 成功返回 0，
     * 失败返回 EOF。
     */
    int close_result = fclose(input);

    /*
     * fclose 调用之后，无论成功还是失败，
     * 原来的 FILE 指针都不应再次使用。
     */
    input = NULL;

    /*
     * 检查文件是否成功关闭。
     */
    check(close_result == 0,
          "Failed to close %s.",
          file_name);

    /*
     * 所有操作成功，返回 0。
     */
    return 0;

/*
 * check 或 check_mem 失败时，
 * 会通过 goto 跳转到这里。
 */
error:
    /*
     * free(NULL) 是安全的。
     *
     * 如果 block 已经成功申请，
     * 这里会将它释放。
     */
    free(block);

    /*
     * 只有文件已经成功打开时，
     * 才需要调用 fclose。
     */
    if (input != NULL)
    {
        fclose(input);
    }

    /*
     * 返回 -1 表示测试失败。
     */
    return -1;
}

/*
 * 测试 sentinel 宏。
 *
 * code 为 1 时表示正常分支。
 *
 * 其他值都会进入不应执行的 default 分支。
 */
static int test_sentinel(int code)
{
    /*
     * 申请 100 字节动态内存。
     */
    char *temporary = malloc(100);

    /*
     * 检查内存是否申请成功。
     */
    check_mem(temporary);

    /*
     * 根据 code 进入不同分支。
     */
    switch (code)
    {
        /*
         * code 等于 1 时，表示正常执行。
         */
        case 1:
            log_info("It worked.");
            break;

        /*
         * 其他值都是意外情况。
         */
        default:
            sentinel("I should not run.");
    }

    /*
     * 正常执行结束后释放内存。
     */
    free(temporary);

    /*
     * 防止继续使用悬空指针。
     */
    temporary = NULL;

    /*
     * 返回 0 表示正常完成。
     */
    return 0;

/*
 * sentinel 和 check_mem 都会跳转到这里。
 */
error:
    /*
     * 发生错误时仍然必须清理已经申请的内存。
     */
    free(temporary);

    /*
     * 返回 -1 表示发生错误。
     */
    return -1;
}

/*
 * 故意测试 check_mem 的失败情况。
 */
static int test_check_mem(void)
{
    /*
     * 故意让 test 成为空指针。
     */
    char *test = NULL;

    /*
     * 因为 test 为 NULL，
     * check_mem 一定会跳转到 error。
     */
    check_mem(test);

    /*
     * 正常情况下不会执行到这里。
     */
    free(test);

    /*
     * 如果意外执行到这里，返回 1。
     */
    return 1;

/*
 * check_mem 会跳转到这里。
 */
error:
    /*
     * 返回 -1 表示成功测试到了错误分支。
     */
    return -1;
}

/*
 * 测试 check_debug。
 */
static int test_check_debug(void)
{
    /*
     * 将 i 设置为 0。
     */
    int i = 0;

    /*
     * i != 0 为假，
     * 因此会输出调试日志并跳转到 error。
     *
     * 定义 NDEBUG 后，
     * 仍然会跳转，但不会输出调试日志。
     */
    check_debug(i != 0,
                "Oops, i was 0.");

    /*
     * 正常情况下不会执行到这里。
     */
    return 0;

/*
 * check_debug 会跳转到这里。
 */
error:
    /*
     * 返回 -1 表示成功进入错误路径。
     */
    return -1;
}

/*
 * 程序入口。
 */
int main(int argc, char *argv[])
{
    /*
     * 程序要求用户提供一个额外参数。
     *
     * argc 应该等于 2：
     *
     * argv[0] 是程序名；
     * argv[1] 是测试文件名。
     */
    check(argc == 2,
          "Need exactly one argument.");

    /*
     * 测试 debug。
     */
    test_debug();

    /*
     * 测试错误日志。
     */
    test_log_err();

    /*
     * 测试警告日志。
     */
    test_log_warn();

    /*
     * 测试普通信息日志。
     */
    test_log_info();

    /*
     * ex20.c 应该存在于当前目录。
     *
     * test_check 应返回 0。
     */
    check(test_check("ex20.c") == 0,
          "test_check failed with ex20.c.");

    /*
     * 运行程序时应传入一个不存在的文件名。
     *
     * 此时 test_check 应返回 -1。
     */
    check(test_check(argv[1]) == -1,
          "test_check unexpectedly opened %s.",
          argv[1]);

    /*
     * code 为 1 时，
     * test_sentinel 应正常返回 0。
     */
    check(test_sentinel(1) == 0,
          "test_sentinel failed for code 1.");

    /*
     * code 为 100 时，
     * test_sentinel 应进入 sentinel 并返回 -1。
     */
    check(test_sentinel(100) == -1,
          "test_sentinel failed for code 100.");

    /*
     * test_check_mem 故意检查 NULL，
     * 因此应该返回 -1。
     */
    check(test_check_mem() == -1,
          "test_check_mem failed.");

    /*
     * test_check_debug 故意检查失败条件，
     * 因此应该返回 -1。
     */
    check(test_check_debug() == -1,
          "test_check_debug failed.");

    /*
     * 所有测试都符合预期，返回成功状态。
     */
    return 0;

/*
 * main 中的任何 check 失败时，
 * 都会跳转到这里。
 */
error:
    /*
     * 返回 1 表示程序执行失败。
     */
    return 1;
}
