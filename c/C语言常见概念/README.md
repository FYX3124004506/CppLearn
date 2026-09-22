# C语言常见概念 — 示例代码

本目录对应笔记 `Note/c语言/c语言常见概念.md` 及其续篇。每个 `.c` 文件对应一个知识点，包含独立的 `main` 函数，应单独编译运行。

## 示例目录

| 序号 | 文件名 | 对应知识点 |
| :---: | :--- | :--- |
| 01 | [01_hello_world.c](01_hello_world.c) | 第一个 C 程序、main 函数和标准输出 |
| 02 | [02_main_forms.c](02_main_forms.c) | main 函数的常见写法对比 |
| 03 | [03_printf_demo.c](03_printf_demo.c) | printf 格式化输出 |
| 04 | [04_ascii_table.c](04_ascii_table.c) | ASCII 编码和字符转换 |
| 05 | [05_string_and_null.c](05_string_and_null.c) | 字符串、\0、strlen 和 sizeof |
| 06 | [06_escape_chars.c](06_escape_chars.c) | 转义字符、八进制和十六进制转义 |
| 07 | [07_keywords_bad.c](07_keywords_bad.c) | 关键字命名错误与正确写法 |
| 08 | [08_input_and_output.c](08_input_and_output.c) | scanf 输入和 printf 输出 |
| 09 | [09_statements_and_control.c](09_statements_and_control.c) | 语句、基本控制结构和跳转语句 |
| 10 | [10_comments.c](10_comments.c) | 单行注释、块注释和行内注释 |

## 编译与运行

### GCC（MinGW）

示例文件包含多个独立的 `main` 函数，不能把所有源文件一次性链接。请在本目录中选择一个文件单独编译：

~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic 01_hello_world.c -o 01_hello_world.exe
~~~

Windows PowerShell 运行：

~~~powershell
.\01_hello_world.exe
~~~

需要输入的 08 号示例可以使用以下测试数据：

~~~powershell
@("小明", "20", "1.75") | .\08_input_and_output.exe
~~~

### Visual Studio

打开 `c.sln`，在解决方案资源管理器中查看 01–10 号示例。工程默认构建 01 号示例，其余文件设置为不参与当前构建；切换示例时应调整对应的 `ExcludedFromBuild`，或直接使用 GCC 单独编译。由于每个文件都有自己的 `main`，不能把所有示例同时链接。

## 编译流程

~~~text
.c 源文件
    ↓ 预处理：处理 #include、#define，并移除注释
    ↓ 编译：翻译为汇编代码
    ↓ 汇编：生成目标文件
    ↓ 链接：与库函数链接，生成可执行文件
~~~
