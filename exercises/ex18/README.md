# Exercise 18: Pointers to Functions

## 学习目标

- 声明和使用函数指针
- 使用typedef简化复杂类型
- 将比较函数作为回调传递
- 使用同一个排序算法实现不同顺序
- 将排序算法本身作为回调传递
- 理解函数指针与普通数据指针的区别
- 使用Valgrind观察空函数指针调用

## 函数指针

普通函数声明：

    int compare(int a, int b);

函数指针声明：

    int (*cmp)(int a, int b);

括号不可省略。

下面的声明含义不同：

    int *cmp(int a, int b);

它表示一个返回int指针的函数。

## typedef

    typedef int (*compare_cb)(int a, int b);

compare_cb成为函数指针类型的别名。

之后可以写：

    compare_cb cmp = sorted_order;

## 回调

bubble_sort接收compare_cb：

    bubble_sort(numbers, count, cmp);

排序过程中通过：

    cmp(target[j], target[j + 1])

调用具体比较函数。

bubble_sort不需要知道cmp是升序、降序还是其他规则。

## 比较函数约定

    cmp(a, b) < 0

表示a应该位于b前面。

    cmp(a, b) == 0

表示二者在排序意义上相等。

    cmp(a, b) > 0

表示a应该位于b后面。

## 避免整数溢出

原书使用：

    return a - b;

极端整数相减可能溢出。

安全写法：

    return (a > b) - (a < b);

## strange_order

使用余数作为比较结果不能保证形成一致的排序关系。

它只用于演示回调可以被替换，不能作为可靠的正式排序规则。

## 数组复制

bubble_sort使用malloc创建目标数组，再通过memcpy复制整数数据。

原始numbers不会被修改。

每个返回的排序数组都由test_sorting负责free。

## 空函数指针

通过NULL函数指针调用时，程序会尝试跳转到无效地址。

Valgrind可能报告：

    Jump to the invalid address
    Address 0x0
    Segmentation fault

调用前应检查：

    cmp != NULL

## 错误签名

如果传入的函数参数或返回类型与compare_cb不一致，编译器会报告函数指针类型不兼容。

函数指针的完整签名必须匹配。

## 查看机器码

函数机器码更适合通过以下命令观察：

    nm -n ex18
    objdump -d -M intel ex18
    xxd -g 1 ex18

把函数指针转换为字符指针读取代码字节不是ISO C保证的可移植行为。

## 排序函数回调

定义：

    typedef int *(*sort_cb)(
        const int *numbers,
        size_t count,
        compare_cb cmp
    );

test_sorting可以同时接收：

- 排序算法回调
- 比较规则回调

因此可以自由组合冒泡排序、插入排序、升序和降序。

## Valgrind结果

正确版本应显示：

    All heap blocks were freed -- no leaks are possible
    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

函数名在大多数表达式中可以转换为函数指针。

函数指针允许把行为作为参数传递，从而让同一段代码使用不同策略。

回调函数的参数类型和返回类型必须与函数指针声明完全匹配。
