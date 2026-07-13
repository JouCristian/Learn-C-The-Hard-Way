# Exercise 6: Types of Variables

## 学习目标

- 认识int、float、double和char
- 理解字符与字符串的区别
- 使用正确的printf格式占位符
- 使用八进制、十六进制和科学计数法输出数字
- 使用Valgrind观察格式参数类型错误

## 基础类型

### int

    int distance = 100;

使用`%d`打印十进制整数。

### float

    float power = 2.345f;

字面量末尾的`f`表示float类型。

float传入printf时会自动提升为double，因此使用`%f`打印。

### double

    double super_power = 56789.4532;

double通常比float拥有更高的精度，同样使用`%f`打印。

### char

    char initial = 'A';

单引号表示单个字符，使用`%c`打印。

### 字符数组

    char first_name[] = "Zed";

双引号表示字符串，使用`%s`打印。字符串末尾存在隐藏的`\0`结束符。

## 单引号和双引号

    'A'

表示单个字符。

    "A"

表示包含字符`A`和结束符`\0`的字符串。

## 格式参数错误实验

将`initial`传给`%s`后，printf会把字符编码当作内存地址。

字符`A`通常对应十进制65，也就是十六进制地址`0x41`。

Valgrind可能报告：

    Invalid read of size 1

程序也可能发生段错误。

## 数字格式

- `%d`：十进制
- `%o`：八进制
- `%x`：小写十六进制
- `%X`：大写十六进制
- `%#o`：带前缀的八进制
- `%#x`：带前缀的十六进制
- `%e`：科学计数法
- `%g`：紧凑浮点格式
- `%.2f`：保留两位小数
- `%8d`：最小宽度为8
- `%08d`：最小宽度为8并补零

## 空字符串

    ""

空字符串没有可见字符，但仍然包含字符串结束符`\0`。

## 本题总结

printf格式占位符必须与参数的真实类型严格匹配。

错误地把字符当字符串地址，可能导致无效内存读取和段错误。

正确版本的Valgrind结果应为：

    ERROR SUMMARY: 0 errors from 0 contexts
