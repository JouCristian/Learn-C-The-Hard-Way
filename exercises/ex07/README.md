# Exercise 7: More Variables, Some Math

## 学习目标

- 学习long和unsigned long
- 理解整数与浮点数混合运算
- 使用科学计数法输出
- 理解char的整数属性
- 观察错误格式占位符导致的内存问题
- 了解有符号和无符号整数的范围

## long类型

    long universe_of_defects =
        1L * 1024L * 1024L * 1024L;

常量末尾的`L`表示long类型。

long使用`%ld`打印。

## 混合类型运算

    double expected_bugs = bugs * bug_rate;

bugs是int，bug_rate是double。运算前，bugs会自动转换为double，因此结果也是double。

## 科学计数法

    printf("%e", part_of_universe);

`%e`使用科学计数法输出浮点数。

    1.117587e-07

表示：

    1.117587 × 10^-7

## char参与运算

    char nul_byte = '\0';
    int care_percentage = bugs * nul_byte;

`\0`的数值为0。

char参与算术运算时会发生整数提升，转换为int，因此结果为：

    100 × 0 = 0

## 百分号

printf格式字符串中使用：

    %%

才能输出真正的百分号字符。

## 错误的%s实验

`%s`要求参数为字符串地址`char *`。

把字符`'A'`传给`%s`时，字符编码65可能被错误地当作地址`0x41`。

Valgrind可能报告：

    Invalid read of size 1

程序也可能发生段错误。

## unsigned

unsigned表示无符号整数，只能保存非负数。

unsigned long使用`%lu`打印。

无符号整数超过最大值后会按照模运算回绕：

    ULONG_MAX + 1UL == 0

而有符号整数溢出属于未定义行为。

## 类型范围

`<limits.h>`提供：

- `LONG_MIN`
- `LONG_MAX`
- `ULONG_MAX`

`sizeof`可以查看类型占用的字节数，并使用`%zu`打印。

## Valgrind结果

正确版本应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

C会在算术表达式中进行类型提升和转换。

格式占位符必须与参数类型匹配，尤其不能把普通字符或整数错误地作为字符串地址传给`%s`。
