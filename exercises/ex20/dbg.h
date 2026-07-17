#ifndef DBG_H
#define DBG_H

/*
 * errno 是部分标准库和系统函数用于记录错误编号的变量。
 */
#include <errno.h>

/*
 * fprintf 和 stderr 的声明位于 stdio.h 中。
 */
#include <stdio.h>

/*
 * strerror 的声明位于 string.h 中。
 */
#include <string.h>

/*
 * 如果编译时定义了 NDEBUG，
 * debug 宏会变成什么都不执行的空操作。
 */
#ifdef NDEBUG

/*
 * 使用 do...while(0) 可以让宏在语法上表现得像一条普通语句。
 *
 * 调用：
 *
 *     debug("hello");
 *
 * 会被展开成一个不会执行任何操作的语句。
 */
#define debug(M, ...)                \
    do                               \
    {                                \
    } while (0)

/*
 * 如果没有定义 NDEBUG，
 * debug 会输出真正的调试信息。
 */
#else

/*
 * M 是类似 printf 格式字符串的参数。
 *
 * ... 表示宏可以继续接收任意数量的参数。
 *
 * __VA_ARGS__ 表示传给 ... 的那些参数。
 *
 * __FILE__ 表示当前源文件名。
 *
 * __LINE__ 表示当前代码行号。
 *
 * __func__ 表示当前函数名。
 *
 * ##__VA_ARGS__ 是 GCC 支持的写法。
 * 当没有附加参数时，它会删除前面的逗号。
 */
#define debug(M, ...)                                            \
    do                                                           \
    {                                                            \
        fprintf(stderr,                                         \
                "[DEBUG] (%s:%d:%s) " M "\n",                   \
                __FILE__,                                       \
                __LINE__,                                       \
                __func__,                                       \
                ##__VA_ARGS__);                                 \
    } while (0)

#endif

/*
 * 如果 errno 为 0，表示当前没有可报告的系统错误，
 * 此时返回字符串 "None"。
 *
 * 如果 errno 不为 0，就通过 strerror 将错误编号
 * 转换成人类可读的错误信息。
 *
 * 这里使用了三元运算符：
 *
 *     条件 ? 条件为真时的结果 : 条件为假时的结果
 */
#define clean_errno()                                            \
    (errno == 0 ? "None" : strerror(errno))

/*
 * 输出错误日志。
 *
 * 信息包括：
 *
 * 文件名
 * 行号
 * 函数名
 * errno 对应的信息
 * 用户提供的错误描述
 */
#define log_err(M, ...)                                          \
    do                                                           \
    {                                                            \
        fprintf(stderr,                                         \
                "[ERROR] (%s:%d:%s: errno: %s) " M "\n",        \
                __FILE__,                                       \
                __LINE__,                                       \
                __func__,                                       \
                clean_errno(),                                  \
                ##__VA_ARGS__);                                 \
    } while (0)

/*
 * 输出警告日志。
 *
 * 警告表示程序仍然可以继续执行，
 * 但出现了值得注意的情况。
 */
#define log_warn(M, ...)                                         \
    do                                                           \
    {                                                            \
        fprintf(stderr,                                         \
                "[WARN] (%s:%d:%s: errno: %s) " M "\n",         \
                __FILE__,                                       \
                __LINE__,                                       \
                __func__,                                       \
                clean_errno(),                                  \
                ##__VA_ARGS__);                                 \
    } while (0)

/*
 * 输出普通信息日志。
 *
 * 普通信息不需要显示 errno。
 */
#define log_info(M, ...)                                         \
    do                                                           \
    {                                                            \
        fprintf(stderr,                                         \
                "[INFO] (%s:%d:%s) " M "\n",                    \
                __FILE__,                                       \
                __LINE__,                                       \
                __func__,                                       \
                ##__VA_ARGS__);                                 \
    } while (0)

/*
 * 检查条件 A 是否成立。
 *
 * 条件成立：
 *     什么都不做，继续执行。
 *
 * 条件不成立：
 *     输出错误日志；
 *     将 errno 重置为 0；
 *     跳转到当前函数中的 error 标签。
 *
 * 使用这个宏的函数必须存在：
 *
 *     error:
 */
#define check(A, M, ...)                                         \
    do                                                           \
    {                                                            \
        if (!(A))                                                \
        {                                                        \
            log_err(M, ##__VA_ARGS__);                           \
            errno = 0;                                          \
            goto error;                                         \
        }                                                        \
    } while (0)

/*
 * sentinel 用于表示程序进入了一个
 * 理论上不应该进入的代码分支。
 *
 * 它不检查条件，调用后直接输出错误并跳转到 error。
 */
#define sentinel(M, ...)                                         \
    do                                                           \
    {                                                            \
        log_err(M, ##__VA_ARGS__);                               \
        errno = 0;                                              \
        goto error;                                             \
    } while (0)

/*
 * 检查动态内存指针是否有效。
 *
 * A 为 NULL 时会输出 Out of memory，
 * 然后跳转到 error 标签。
 */
#define check_mem(A)                                             \
    check((A) != NULL, "Out of memory.")

/*
 * check_debug 和 check 类似。
 *
 * 区别是它使用 debug 输出信息。
 *
 * 如果定义了 NDEBUG：
 *
 *     条件仍然会检查；
 *     仍然会跳转到 error；
 *     但不会打印调试信息。
 */
#define check_debug(A, M, ...)                                   \
    do                                                           \
    {                                                            \
        if (!(A))                                                \
        {                                                        \
            debug(M, ##__VA_ARGS__);                             \
            errno = 0;                                          \
            goto error;                                         \
        }                                                        \
    } while (0)

#endif
