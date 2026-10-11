# C 语言数据类型和变量

本目录用独立、可运行的程序说明变量声明与初始化、赋值、块作用域、名称遮蔽，以及自动存储期和静态存储期的基本区别。

## 文件

| 文件 | 内容 |
| :--- | :--- |
| `01_variables_and_scope.c` | 展示不同类型变量的定义、赋值、块内外同名变量，以及静态存储期对象的初始化和保留状态 |

## 编译和运行

在本目录中使用 GCC 编译：

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic 01_variables_and_scope.c -o 01_variables_and_scope.exe
```

在 Windows PowerShell 中运行：

```powershell
.\01_variables_and_scope.exe
```

程序输出：

```text
age=18, price=12.50, grade=A
score after assignment=95
inner count=12
outer count after block=5
global total_visits=0
visit=1
visit=2
```

## 阅读要点

- `score` 先声明、后赋值，并且只在赋值后读取。未初始化的自动局部变量具有不确定值，不能在赋值前读取。
- 花括号内的 `count` 只在该代码块中可见，并遮蔽外层同名变量；离开代码块后，外层变量仍为 `5`。
- 文件作用域对象 `total_visits` 具有静态存储期，未显式初始化时会被初始化为零。
- 函数内的 `visit_count` 也具有静态存储期，因此两次调用之间会保留其值。
- `int`、`double` 和 `char` 用于展示不同变量类型；`printf` 的格式说明分别匹配对应的参数类型。

## 扩展示例

新增程序时，为每个可独立运行的知识点使用单独的 `.c` 文件；文件名采用连续编号和英文主题名。编译时开启常见警告，运行后检查输出，并避免提交可执行文件、目标文件和 IDE 缓存。
