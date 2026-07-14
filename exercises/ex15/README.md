# Exercise 15: Pointers Dreaded Pointers

## 学习目标

- 理解指针保存的是内存地址
- 学习取地址和解引用
- 学习指针算术
- 理解数组与指针的联系和区别
- 使用二级指针处理字符串数组
- 使用指针遍历命令行参数
- 使用Valgrind观察错误指针访问

## 指针

    int *cur_age = ages;

cur_age是指向int的指针，保存ages首元素的地址。

    cur_age

表示地址。

    *cur_age

表示该地址中的int值。

## 取地址

    &ages[0]

表示ages[0]的地址。

    int *ptr = &ages[0];

将该地址保存到ptr。

## 数组下标与指针

以下表达式等价：

    ages[i]
    *(ages + i)
    *(cur_age + i)
    cur_age[i]

下标操作本质上通过指针算术和解引用完成。

## 指针算术

指针加1会移动到下一个同类型元素，而不是简单增加一个字节。

    int指针 + 1

地址增加sizeof(int)字节。

    字符串指针数组的指针 + 1

地址增加一个字符串指针的大小。

## 字符串指针数组

    const char *names[];

names是一个数组，每个元素都是指向字符串的指针。

    const char **cur_name = names;

cur_name是指向字符串指针的指针。

    *cur_name

是当前字符串地址。

    **cur_name

是当前字符串的第一个字符。

## 数组与指针不同

    sizeof(ages)

得到整个数组的大小。

    sizeof(cur_age)

只得到指针变量本身的大小。

数组在大多数表达式中会转换为首元素指针，但数组本身不是指针。

## 尾后指针

    ages + count

可以形成指向数组最后一个元素之后的位置的指针。

它可以用于比较，但不能解引用。

## 指针相减

两个指针指向同一个数组时：

    cur_age - ages

结果是相隔的元素数量，不是字节数量。

结果类型为ptrdiff_t，使用`%td`打印。

## 地址打印

使用：

    printf("%p", (void *)pointer);

`%p`用于打印地址。

`&names[i]`是指针槽位的地址，`names[i]`是字符串的地址。

## 函数和指针

函数接收数组时，实际接收到的是数组首元素的指针。

指针本身不包含数组长度，因此必须额外传入count。

## 倒序遍历

安全方式是从尾后位置开始：

    ptr = array + count;

然后先递减，再解引用：

    ptr--;
    printf("%d", *ptr);

这样不会形成array - 1这样的非法指针。

## 错误强制转换

把int数组强制转换为字符串指针数组，并不会真正转换数据。

它只是使用错误的类型解释同一片内存，可能导致：

    Invalid read of size 1
    Segmentation fault

强制转换能够消除部分编译器诊断，但不能使错误操作变得合法。

## Valgrind结果

最终正确程序应显示：

    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

指针是带有类型信息的内存地址。

数组和指针可以使用相似的访问语法，但它们不是同一种对象。

使用指针时必须保证地址有效、类型正确、对象仍然存在并且没有越界。
