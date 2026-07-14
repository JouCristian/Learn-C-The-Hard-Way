# Exercise 13: Switch Statement

## 学习目标

- 学习switch、case、default和break
- 理解字符可以作为整数参与switch判断
- 理解贯穿执行
- 使用二维下标遍历命令行字符串
- 使用非0退出状态表示错误
- 将switch改写为if分支

## 参数检查

程序要求用户恰好输入一个参数。

    argc == 2

其中：

    argv[0]

通常是程序名，

    argv[1]

是用户输入的字符串。

参数数量错误时执行：

    return 1;

向操作系统返回非0退出状态。

## 字符串遍历

    for (i = 0; argv[1][i] != '\0'; i++)

逐字符遍历argv[1]，遇到字符串结束符`\0`时停止。

`argv[1][i]`表示用户第一个参数中的第i个字符。

## switch

基本结构：

    switch (表达式)
    {
        case 常量:
            代码;
            break;

        default:
            代码;
            break;
    }

switch表达式必须是整数类型或枚举类型。

char本质上是整数类型，因此可以作为switch表达式。

## 多个case共用代码

    case 'a':
    case 'A':
        printf("A");
        break;

字符a和A都会执行同一段代码。

这是有意使用的贯穿执行。

## break

break会退出当前switch，但不会退出外层for循环。

忘记break可能导致程序继续执行后续case。

例如E分支缺少break时，可能同时输出E和I。

## default

当所有case都不匹配时执行default。

删除default后，未识别字符会被静默忽略。

## switch的实现

switch在语言语义上并不保证使用跳转表。

编译器可能生成：

- 连续比较
- 二分判断
- 跳转表
- 多种策略的组合

程序不应该依赖某种具体底层实现。

## Y分支

break位于if外部时，位置小于等于2的Y会被静默忽略。

如果把break移进if，则位置小于等于2的Y会贯穿到default，被打印为非元音。

## 大小写转换

可以通过算术把ASCII大写字母转换为小写：

    letter = letter - 'A' + 'a';

更通用的程序应使用`<ctype.h>`中的tolower。

## 处理所有参数

使用两层for循环：

- 外层遍历argv中的参数
- 内层遍历每个字符串中的字符

## switch与if

switch适合把同一个整数表达式与多个固定常量比较。

if更适合范围判断和多个条件组合。

## Valgrind结果

最终正确程序应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

switch用于根据一个整数值选择不同分支。

case后需要使用break避免意外贯穿，default用于处理未知值。

退出状态0通常表示成功，非0表示错误。
