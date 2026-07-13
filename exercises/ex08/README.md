# Exercise 8: Sizes and Arrays

## 学习目标

- 使用sizeof查看类型和对象的大小
- 理解数组的连续内存结构
- 理解数组下标从0开始
- 理解C字符串和`\0`
- 学习计算数组元素数量
- 观察字符串缺少结束符和数组越界的后果

## sizeof

`sizeof`返回对象或类型占用的字节数，返回类型为`size_t`，应使用`%zu`打印。

例如：

    sizeof(int)
    sizeof(areas)
    sizeof(areas[0])

## 整数数组

    int areas[] = {10, 12, 13, 14, 20};

数组拥有5个元素，有效下标为：

    0, 1, 2, 3, 4

如果一个int占4字节，则整个数组占：

    5 × 4 = 20字节

元素数量可以通过以下表达式计算：

    sizeof(areas) / sizeof(areas[0])

## 字符串

    char name[] = "Zed";

编译器会自动在末尾添加`\0`，实际内容为：

    'Z', 'e', 'd', '\0'

因此`sizeof(name)`为4。

## full_name

手动构造字符数组时，需要主动加入：

    '\0'

否则它只是字符数组，而不是合法的C字符串。

## 数组赋值实验

数组元素可以通过下标修改：

    areas[0] = 100;
    name[0] = 'B';
    full_name[0] = 'N';

char可以赋值给int数组元素，因为char本质上是一种小整数类型。

## 缺少字符串结束符

删除full_name末尾的`\0`后，`printf("%s")`会继续读取数组之外的内存，直到偶然遇到一个0字节。

程序可能正常、输出乱码或崩溃，这属于未定义行为。

Valgrind可能报告：

    Invalid read of size 1

## 数组越界

areas只有5个元素，访问：

    areas[10]

属于数组越界和未定义行为。

Valgrind可能报告：

    Invalid read of size 4

size 4表示程序尝试读取一个通常占4字节的int。

## 全局数组和局部数组

局部数组通常存放在栈中，全局数组位于静态存储区。

缺少`\0`时，两种存储位置可能表现不同，但读取数组外部始终是错误的。

## 不同系统的类型大小

常见64位Linux采用LP64模型：

- int为4字节
- long为8字节
- 指针为8字节

64位Windows常采用LLP64模型，其中long仍然为4字节。

## 本题总结

数组是一块连续内存，下标必须位于合法范围内。

C字符串本质上是char数组，并且必须以`\0`结束。

最终正确程序的Valgrind结果为：

    ERROR SUMMARY: 0 errors from 0 contexts
