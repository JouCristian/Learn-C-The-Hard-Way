# 使用 GCC 编译器。
CC=gcc

# 开启常见警告、额外警告和调试信息。
#
# 使用 gnu11 是因为本题的 ##__VA_ARGS__
# 属于 GCC 支持的可变参数宏扩展。
CFLAGS=-Wall -Wextra -g -std=gnu11

# 默认同时构建调试版本和关闭调试信息的版本。
all: ex20 ex20_release

# 调试版本依赖 ex20.c 和 dbg.h。
ex20: ex20.c dbg.h
	$(CC) $(CFLAGS) ex20.c -o ex20

# 发布版本在编译时定义 NDEBUG。
#
# 这会关闭 debug 宏的输出，
# 但 log_err、log_warn 和 log_info 仍会保留。
ex20_release: ex20.c dbg.h
	$(CC) $(CFLAGS) -DNDEBUG ex20.c -o ex20_release

# 删除编译生成的文件。
clean:
	rm -f ex20 ex20_release

# 声明这些目标不是普通文件。
.PHONY: all clean
