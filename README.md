# Learn C the Hard Way 学习记录

本仓库用于记录《Learn C the Hard Way》以及“一生一芯”PA0 E4相关的C语言学习过程。

## 学习范围

- 练习 0～22
- 练习 24～25
- 练习 27～33

## 记录内容

每一道练习尽量保存：

- C源代码
- Makefile
- 编译日志
- 运行日志
- Valgrind检查结果
- 错误分析
- 附加题答案
- Git提交记录

## 目录说明

- `exercises/`：各章节练习
- `notes/`：知识总结
- `session-logs/`：终端操作记录
- `tools/`：辅助脚本

## 编译要求

默认使用较严格的GCC编译参数：

```bash
gcc -Wall -Wextra -Werror -g source.c -o program
