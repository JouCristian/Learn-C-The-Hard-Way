# Exercise 16: Structs and Pointers to Them

## 学习目标

- 创建和使用结构体
- 使用结构体指针和箭头运算符
- 使用malloc申请动态内存
- 使用strdup复制字符串
- 使用free释放内存
- 使用Valgrind检查内存泄漏
- 对比栈结构体和堆结构体

## struct Person

    struct Person
    {
        char *name;
        int age;
        int height;
        int weight;
    };

结构体把多个不同类型的成员组合成一个新的复合类型。

## 点号和箭头

普通结构体对象使用：

    person.age

结构体指针使用：

    person_ptr->age

箭头表达式等价于：

    (*person_ptr).age

## malloc

    struct Person *who = malloc(sizeof(*who));

malloc申请能够容纳一个Person结构体的动态内存。

成功时返回地址，失败时返回NULL。

malloc得到的内存没有自动初始化，因此所有成员都应在使用前赋值。

## strdup

    who->name = strdup(name);

strdup会申请新内存并复制字符串，包括末尾的`\0`。

返回的字符串必须使用free释放。

## 内存所有权

每个Person对象包含两块动态内存：

1. malloc创建的结构体
2. strdup创建的名字字符串

销毁时应先执行：

    free(who->name);

再执行：

    free(who);

释放结构体不会自动释放其成员所指向的内存。

## assert

    assert(who != NULL);

条件为假时，assert会打印错误并终止程序。

`-g`只生成调试信息，不控制assert。

定义：

    NDEBUG

才会禁用assert。

assert适合检查程序内部不变量，不应替代正式的错误处理。

## NULL实验

向Person_destroy传入NULL时，assert会失败并调用abort。

禁用assert后，访问`who->name`会导致空指针解引用。

向Person_print传入NULL时，Valgrind可能报告：

    Invalid read
    Address 0x0
    Segmentation fault

## 内存泄漏实验

忘记调用Person_destroy时，结构体和名字字符串都会泄漏。

忘记执行：

    free(who->name);

时，名字字符串会泄漏。

使用：

    --leak-check=full
    --show-leak-kinds=all

可以查看详细泄漏位置。

## 结构体布局

结构体成员按顺序存储，但编译器可能加入填充字节以满足内存对齐。

可以使用：

    sizeof(struct Person)
    offsetof(struct Person, member)

查看结构体大小和成员偏移。

具体布局取决于平台和ABI。

## 栈版本

栈上创建结构体：

    struct Person joe = {
        "Joe Alex",
        32,
        64,
        140
    };

使用点号访问：

    joe.age

局部结构体会在函数结束时自动销毁，不需要malloc和free。

按值传递结构体会复制整个结构体。

## 悬空指针

free之后，指针仍保存旧地址，但不能再使用。

释放后将指针设置为NULL可以降低误用风险：

    Person_destroy(joe);
    joe = NULL;

## Valgrind结果

最终正确程序应显示：

    All heap blocks were freed -- no leaks are possible
    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

结构体把多个成员组织成一个整体。

malloc创建的动态内存必须由free释放。

结构体成员如果还指向其他动态内存，需要分别释放。

普通结构体使用`.`，结构体指针使用`->`。
