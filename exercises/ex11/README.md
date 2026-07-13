# Exercise 11: While-Loop and Boolean Expressions

## 学习目标

- 理解C中的真假
- 使用while循环
- 理解循环变量的初始化、判断和更新
- 使用while倒序遍历数组
- 安全地把argv元素赋给states
- 理解指针复制不等于字符串内容复制

## C中的布尔表达式

条件表达式通常产生整数结果：

- 0表示假
- 非0表示真

比较表达式如：

    i < argc

成立时通常得到1，不成立时得到0。

现代C还提供`_Bool`，并可通过`<stdbool.h>`使用：

    bool
    true
    false

但if和while仍然按照0与非0判断真假。

## while循环

基本形式：

    while (条件)
    {
        循环体;
    }

只要条件为非0，循环体就会继续执行。

完整的数组遍历通常需要：

1. 初始化循环变量
2. 检查边界
3. 更新循环变量

## 忘记重置i

第一个循环结束后，i等于argc。

如果第二个循环前不执行：

    i = 0;

第二个循环可能跳过部分元素，也可能完全不执行。

## 忘记i++

如果循环中不更新i，条件可能一直为真，从而形成无限循环。

可以按`Ctrl+C`终止，也可以使用`timeout`限制运行时间。

## 未初始化i

    int i;

在赋值前使用i属于未定义行为。

Valgrind可能报告：

    Conditional jump or move depends on uninitialised value(s)

## 倒序遍历

安全的倒序方式：

    i = argc;

    while (i > 0)
    {
        i--;
        printf("%s\n", argv[i]);
    }

必须先递减，再访问，避免访问`argv[argc]`。

## 安全复制argv到states

循环条件：

    arg_index < argc &&
    state_index < num_states

同时限制源数组和目标数组下标，避免越界。

## 指针赋值

    states[state_index] = argv[arg_index];

只复制字符串地址，没有复制字符串字符。

赋值后，两个指针指向同一块字符串内容。

真正复制字符串需要新的存储空间以及字符复制操作。

## Valgrind结果

最终主程序和附加题程序都应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

while循环需要程序员自己负责初始化和更新循环变量。

遍历数组时必须保持下标小于真实元素数量。

指针赋值只是复制地址，并不等于复制字符串内容。
