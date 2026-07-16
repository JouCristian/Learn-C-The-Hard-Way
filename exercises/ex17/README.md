# Exercise 17: Heap and Stack Memory Allocation

## 学习目标

- 使用多个结构体构造小型数据库
- 使用calloc和free管理堆内存
- 使用FILE指针进行二进制文件读写
- 理解栈变量与堆对象的关系
- 使用errno和perror报告系统错误
- 使用strtol安全解析整数
- 修复定长字符串复制问题
- 使用Valgrind检查内存泄漏
- 编写Shell自动化测试

## 数据结构

数据库包含三层结构：

    Connection
    ├── FILE *file
    └── Database *db
        └── Address rows[MAX_ROWS]

Address包含：

    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];

字符数据直接存放在结构体中，因此整个Database可以一次写入文件。

## 堆和栈

main中的conn是局部指针变量，通常位于栈中。

conn指向的Connection和Database由calloc创建，位于动态存储区，必须使用free释放。

    conn                 堆
      └── Connection
            └── db
                 └── Database

局部变量会在函数结束时自动失效，但堆内存不会自动因为指针离开作用域而释放。

## 动态内存

程序进行了两次动态分配：

    calloc(1, sizeof(*conn));
    calloc(1, sizeof(*conn->db));

关闭时对应：

    free(conn->db);
    free(conn);

一次成功的动态分配通常应对应一次free。

## 文件模式

创建数据库使用：

    wb+

打开已有数据库使用：

    rb+

二进制模式更适合保存结构体的原始字节。

创建动作会截断已有文件，因此会清空旧数据库。

## fread和fwrite

读取：

    fread(conn->db,
          sizeof(*conn->db),
          1,
          conn->file);

写入：

    fwrite(conn->db,
           sizeof(*conn->db),
           1,
           conn->file);

返回值是完整读取或写入的对象数量。

## errno和perror

errno只应在一个明确失败并会设置errno的函数之后检查。

perror会根据errno输出系统错误信息。

普通逻辑错误与系统错误分别使用die和die_errno处理，避免打印过期的errno。

## strtol

原书使用atoi，但atoi无法可靠报告转换失败。

strtol可以检查：

- 是否读取到数字
- 是否存在多余字符
- 是否发生范围错误
- ID是否为负数
- ID是否超出MAX_ROWS

## strncpy问题

当源字符串长度大于等于目标容量时，strncpy不会保证写入结尾的`\0`。

安全版本先检查长度，再复制：

    if (length >= destination_size)
        error;

    memcpy(destination,
           source,
           length + 1);

MAX_DATA为512时，最多保存511个可见字符，最后一个字节留给`\0`。

## 结构体文件大小

在常见64位Linux中：

    sizeof(struct Address) = 1032
    sizeof(struct Database) = 103200

数据库文件大小通常等于：

    sizeof(struct Database)

具体结果应通过sizeof和offsetof实际验证。

## 文件格式限制

直接写入结构体存在平台相关问题：

- 字节序
- int表示方式
- 结构体填充
- 文件版本
- 结构体修改后的兼容性

因此这种格式适合作为学习实验，不适合长期跨平台存储。

## 自动化测试

test_ex17.sh使用：

    set -euo pipefail

脚本会自动执行创建、添加、查询、删除、非法参数测试，并验证输出内容。

## Valgrind

对创建、添加、查询、列出和删除操作分别运行Valgrind。

正确结果应包含：

    All heap blocks were freed -- no leaks are possible
    ERROR SUMMARY: 0 errors from 0 contexts

## 本题总结

栈上的指针变量和它所指向的堆对象是两件不同的东西。

指针离开作用域不会自动释放堆内存。

文件读写必须检查返回值，字符串必须保证存在`\0`，命令行整数应使用能够报告错误的转换函数。
