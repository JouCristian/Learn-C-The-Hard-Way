# 使用GCC编译器。
CC=gcc

# 开启常见警告、额外警告和调试信息。
#
# dbg.h使用GCC的可变参数宏扩展，
# 所以使用gnu11模式。
CFLAGS=-Wall -Wextra -g -std=gnu11

# 默认构建ex25。
all: ex25

# 源文件或dbg.h变化时重新构建程序。
ex25: ex25.c dbg.h
	$(CC) $(CFLAGS) ex25.c -o ex25

# 删除生成的程序。
clean:
	rm -f ex25

# 声明all和clean不是普通文件。
.PHONY: all clean
