# Exercise 2: Make Is Your Python Now

## 学习目标

- 理解Make的基本用途
- 学会创建Makefile
- 使用CFLAGS配置编译参数
- 创建clean和all目标
- 理解Makefile命令前必须使用Tab
- 了解make的隐式规则

## Makefile

最终使用：

    CFLAGS=-Wall -Wextra -g

    all: ex1

    clean:
    	rm -f ex1

    .PHONY: all clean

注意：`rm -f ex1`前面是真正的Tab字符。

## CFLAGS

- `-Wall`：启用常见警告
- `-Wextra`：启用更多警告
- `-g`：加入调试信息，方便GDB使用

## make的隐式规则

Makefile中没有明确写出如何构建`ex1`，但make发现存在`ex1.c`，会通过内置规则执行类似命令：

    cc -Wall -Wextra -g ex1.c -o ex1

## all目标

    all: ex1

表示构建`all`之前，需要先构建`ex1`。

由于`all`是Makefile中的第一个目标，直接执行：

    make

就会构建`ex1`。

## clean目标

    clean:
    	rm -f ex1

执行：

    make clean

会删除编译生成的可执行文件。

## missing separator错误

删除命令前面的Tab后，make提示：

    missing separator

Makefile中的命令必须使用Tab缩进，不能直接使用普通空格代替。

## .PHONY

    .PHONY: all clean

表示`all`和`clean`是伪目标，而不是真实文件。

## 本题总结

Make不仅可以编译程序，还可以执行清理、测试和其他自动化任务。它通过目标、依赖和命令描述项目的构建过程。
