/* =========================================================
 * 文件名：02_main_forms.c
 * 描述：main 函数的常见写法对比
 * 说明：只有第一个 main 会实际编译，其余写法均为注释示例。
 * ========================================================= */

#include <stdio.h>

/*
 * 推荐写法：int main(void)
 * void 明确表示函数不接收参数，int 表示向操作系统返回状态码。
 */
int main(void) {
    printf("标准写法：int main(void)\n");
    return 0;
}

/*
 * 以下内容只用于对比，不能与上面的 main 同时取消注释。
 */

/* 1. 空参数列表写法：可以工作，但没有显式写出 void。 */
// int main() {
//     printf("空参数列表：int main()\n");
//     return 0;
// }

/*
 * 2. 命令行参数写法：适合需要读取启动参数的程序。
 * argc 表示参数数量，argv 表示参数字符串数组。
 */
// int main(int argc, char *argv[]) {
//     printf("参数个数：%d\n", argc);
//     for (int i = 0; i < argc; ++i) {
//         printf("参数 %d：%s\n", i, argv[i]);
//     }
//     return 0;
// }

/* 3. void main() 不是 C 标准规定的 main 函数形式，不推荐使用。 */
// void main(void) {
//     printf("非标准写法\n");
// }

/*
 * 4. main() { ... } 依赖旧式 C 语法。
 * 现代 C 标准不应省略返回类型，也不建议省略参数说明。
 */
// main() {
//     return 0;
// }
