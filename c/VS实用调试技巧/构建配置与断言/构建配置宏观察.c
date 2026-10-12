/*
 * 文件用途：观察当前构建是否定义 _DEBUG 和 NDEBUG，并展示标准断言。
 * 输入：无外部输入；断言检查在断言外完成状态更新后的固定结果。
 * 输出：宏状态、断言外更新的值和断言之后的提示。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic -D_DEBUG -g -O0 构建配置宏观察.c -o build_mode.exe
 * 另一种构建：gcc -std=c11 -Wall -Wextra -Wpedantic -DNDEBUG -O2 构建配置宏观察.c -o build_mode_release.exe
 * 保留调试信息的优化构建：gcc -std=c11 -Wall -Wextra -Wpedantic -DNDEBUG -O2 -g 构建配置宏观察.c -o build_mode_release_symbols.exe
 * 运行：分别启动各配置生成的程序；在 Visual Studio 中可切换项目配置并查看预处理器定义。
 */
#include <assert.h>
#include <stdio.h>


/*
 * 用途：输出编译时宏状态，便于在断点处观察配置差异。
 * 参数：无。
 * 返回值：无。
 * 异常/错误：C 不使用异常；本函数没有错误分支。
 * 副作用：向标准输出写两行文字。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：由 main 调用；输出取决于编译命令或项目属性定义的宏。
 */
static void print_build_markers(void)
{
#if defined(_DEBUG)
    puts("_DEBUG 已定义");
#else
    puts("_DEBUG 未定义");
#endif

#if defined(NDEBUG)
    puts("NDEBUG 已定义：assert 不执行检查");
#else
    puts("NDEBUG 未定义：assert 会检查表达式");
#endif
}

/*
 * 用途：作为程序入口，打印配置标记并执行无副作用的断言。
 * 参数：无。
 * 返回值：正常运行返回 0；若断言启用且表达式为假，运行库会终止程序。
 * 异常/错误：C 不使用异常；本例断言表达式恒真，不产生错误状态。
 * 副作用：调用 print_build_markers 写标准输出，并写出最后一行提示。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：运行 build_mode.exe；程序不需要命令行参数或标准输入。
 */
int main(void)
{
    print_build_markers();

    /* 不在断言里放副作用；NDEBUG 定义后，断言表达式不会求值。 */
    int checked_value = 2;
    ++checked_value;
    assert(checked_value == 3);
    printf("assert 外完成的状态更新：%d\n", checked_value);
    puts("断言语句之后继续执行");
    return 0;
}
