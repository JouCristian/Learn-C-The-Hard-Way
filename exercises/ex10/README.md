# Exercise 10: Arrays of Strings, Looping

## 学习目标

- 理解argc和argv
- 学习for循环
- 理解字符串数组
- 观察数组元素数量错误导致的越界
- 学习NULL和指针赋值

## argc和argv

main函数：

    int main(int argc, char *argv[])

`argc`表示命令行参数数量，`argv`是参数字符串数组。

执行：

    ./ex10 one two

通常得到：

    argc = 3
    argv[0] = "./ex10"
    argv[1] = "one"
    argv[2] = "two"

程序名本身也计算在参数数量中。

## for循环

基本结构：

    for (初始化; 条件; 更新)
    {
        循环体;
    }

执行顺序：

1. 执行初始化
2. 判断条件
3. 条件为真时执行循环体
4. 执行更新
5. 返回条件判断

## 从1开始遍历argv

    for (i = 1; i < argc; i++)

跳过了`argv[0]`，因为它通常是程序名称。

从0开始也是合法的，只是会同时输出程序名。

## 字符串数组

    char *states[] = {
        "California",
        "Oregon",
        "Washington",
        "Texas"
    };

`states`是一个指针数组，每个元素都指向一个C字符串。

    states[0]

是第一个字符串。

    states[0][0]

是第一个字符串的第一个字符。

## 数组越界实验

将`num_states`从4改为10后，循环会访问不存在的元素。

在64位系统中，Valgrind可能报告：

    Invalid read of size 8

因为字符串数组的每个元素通常是一个8字节指针。

垃圾指针被传给`%s`后，还可能产生：

    Invalid read of size 1
    Segmentation fault

## for中的逗号

可以在初始化和更新区域使用逗号执行多个表达式：

    for (i = 0, j = 10; i < j; i++, j--)

三个主要区域仍然使用分号分隔。

## NULL

`NULL`表示空指针，不指向有效对象。

将`NULL`直接传给`%s`属于未定义行为。某些系统可能输出`(null)`，另一些系统可能崩溃。

安全使用前应检查：

    if (states[i] != NULL)

## states和argv赋值

    states[0] = argv[1];

只改变指针指向，不复制字符串内容。

反过来：

    argv[1] = states[0];

也是指针赋值。

访问`argv[1]`前必须先确认：

    argc > 1

## 参数数量

只要循环条件使用`i < argc`，传入大量参数通常不会造成数组越界。

操作系统会准确设置`argc`，并且命令行总长度还受`ARG_MAX`限制。

## Valgrind结果

最终正确版本应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

`argv`本身就是一个字符串数组。

for循环通过初始化、条件判断和更新重复执行代码。

遍历数组时必须保证循环上限与真实元素数量一致。
