/* 展示注释、字符串中的注释符，以及块注释不能嵌套。 */
#include <stdio.h>

/* 受控编译反例：取消下方两行的 // 后，内层结束标记会先关闭外层注释，剩余文本导致编译诊断。 */
// /* outer comment begins
//    /* inner-looking comment */ int = 3;

int main(void) {
    int count = 2;

    count += 3; // += 把右侧数加到 count 并存回，等价于 count = count + 3。

    printf("count = %d\n", count);

    /* 引号内的注释符是字符串内容，不会开始或结束源码注释。 */
    printf("// and /* */ inside a string are text\n");
    return 0;
}

// 示例输入：无；count 固定从 2 开始。
// 预期输出：
// count = 5
// // and /* */ inside a string are text
// 编译说明：活动代码可编译；上方受控反例取消 // 后会产生诊断。
// 关键结果：源码注释不执行，字符串中的同样字符会照常输出。
