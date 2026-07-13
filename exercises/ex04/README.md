# Exercise 4: Introducing Valgrind

## 故意设置的错误

原程序包含两个主要错误：

1. `printf`中的`%d`没有对应的`age`参数。
2. 局部变量`height`没有初始化就被使用。

## 编译器警告

编译器发现：

- `%d`缺少匹配的`int`参数；
- `age`被定义但未使用；
- `height`在初始化前被使用。

## Valgrind检查

执行：

    valgrind --track-origins=yes ./ex4

Valgrind发现未初始化的`height`被传入`printf`，并在整数转换和条件判断中被多次使用。

`--track-origins=yes`进一步指出，该未初始化值来自`main`函数的栈空间。

## 为什么有多条错误

同一个未初始化值进入`printf`后，被内部多个步骤使用，因此产生多条错误记录。

## HEAP SUMMARY

程序虽然没有直接调用`malloc`，但标准输出库可能创建缓冲区，因此报告一次内部申请和释放。

所有堆块均已释放，说明没有内存泄漏，但这并不代表没有其他错误。

## 修复方式

    int age = 10;
    int height = 72;

    printf("I am %d years old.\n", age);
    printf("I am %d inches tall.\n", height);

## 修复结果

编译器不再产生警告，Valgrind最终显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

编译器负责发现格式参数不匹配和明显的未初始化使用，Valgrind负责观察程序运行时如何使用错误数据。

正确流程是：

    处理编译器警告
    → 运行程序
    → 使用Valgrind检查
    → 修复到0 errors
