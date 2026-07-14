# Exercise 14: Writing and Using Functions

## 学习目标

- 编写和调用自定义函数
- 理解函数声明、定义和调用
- 理解int与void返回类型
- 使用ctype.h中的字符分类函数
- 使用strlen传递字符串长度
- 使用Valgrind观察越界访问

## 函数调用链

    main
    → print_arguments
    → print_letters
    → can_print_it
    → isalpha / isblank

## 前向声明

    int can_print_it(int ch);
    void print_letters(const char arg[]);

前向声明提前告诉编译器函数的名称、参数和返回类型。

如果函数在被调用之前已经完成定义，则不一定需要额外的前向声明。

## 函数定义

一般结构：

    返回类型 函数名(参数列表)
    {
        函数体;
    }

`int`函数返回整数值，`void`函数不返回值。

## const参数

    void print_letters(const char arg[]);

const表示函数只读取字符串，不通过arg修改字符内容。

在函数参数中，`char arg[]`会被调整为`char *arg`。

## ctype.h

本题使用：

- `isalpha`：判断字母
- `isblank`：判断空格或水平Tab

这些函数返回0或非0，非0不保证一定等于1。

参数必须是EOF，或者能够表示为unsigned char的值，因此使用：

    int ch = (unsigned char)arg[i];

## 删除前向声明

删除声明后，编译器可能报告：

    implicit declaration of function
    conflicting types

说明函数在调用前没有被正确声明。

## argc加1实验

将：

    print_arguments(argc, argv);

改为：

    print_arguments(argc + 1, argv);

循环会访问`argv[argc]`。这个元素是NULL，传给print_letters后会发生空指针访问。

Valgrind可能报告：

    Invalid read of size 1
    Address 0x0
    Segmentation fault

## strlen

strlen声明在`<string.h>`中：

    size_t strlen(const char *s);

它返回`\0`之前的字符数量，不包含`\0`。

返回类型为size_t，应使用`%zu`打印。

## 减少函数数量

can_print_it可以合并进print_letters，但保留它可以让判断逻辑更容易阅读和测试。

函数并不是越多越好，也不是越少越好，目标是让每个函数职责清晰。

## K&R语法

旧式函数会把参数类型写在参数列表之后。

这种语法已经过时，不应在现代C代码中使用。

## Valgrind结果

正确版本应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

函数可以把程序拆分为职责清晰的模块。

函数在调用前必须拥有正确的声明，参数和返回类型必须一致。

字符串长度、字符分类和数组边界都需要程序员明确管理。
