/*
 * 文件用途：对比普通形参和指针形参的值传递，以及通过指针修改调用者对象。
 * 输入：无；使用固定整数，并额外检查空指针边界。
 * 输出：普通形参副本、指针形参重指向结果、调用者变量变化和空指针拒绝状态。
 * 独立运行：gcc -std=c11 -Wall -Wextra -Wpedantic 值传递与指针.c -o 值传递与指针.exe
 */
#include <stddef.h>
#include <stdio.h>

/*
 * 用途：修改并输出本次调用的局部形参副本。
 * 参数：value 是调用者整数值的一份副本，单位和值域与 int 相同。
 * 返回值：无。
 * 异常/错误：C 不使用异常；printf 的输出错误不检查。
 * 副作用：修改函数内部的形参副本，并向标准输出显示副本值；不改变调用者变量。
 * 复杂度：时间 O(1)，辅助空间 O(1)；输出文本长度固定。
 * 示例：main 调用 change_copy(score) 时内部显示 99，返回后 score 仍为 10。
 */
void change_copy(int value)
{
    value = 99;
    printf("inside copied parameter: %d\n", value);
}

/*
 * 用途：通过传入的地址修改调用者拥有的整数对象。
 * 参数：target 是可空的 int 指针；非空时必须指向仍在生命周期内的可写 int 对象。
 *       new_value 是要写入的 int 值。
 * 返回值：写入成功返回 1；target 为空时返回 0。
 * 异常/错误：C 不使用异常；先检查空指针，拒绝后不解引用。
 * 副作用：target 非空时修改它所指向的调用者对象。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 调用 set_target(&score, 99) 后，score 变成 99；set_target(NULL, 7) 安全返回 0。
 */
int set_target(int *target, int new_value)
{
    if (target == NULL) {
        return 0;
    }

    *target = new_value;
    return 1;
}

/*
 * 用途：重指向本次调用中的指针形参副本，并修改 replacement 指向的对象。
 * 参数：target 是调用者指针值的副本；replacement 是非空且指向可写 int 对象的指针。
 * 返回值：两个指针都非空时返回 1；否则返回 0。
 * 异常/错误：C 不使用异常；空指针会被拒绝，且不会解引用。
 * 副作用：replacement 指向的对象被设为 55；调用者的指针变量不变。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 传入 score_address 与 alternate_score 的地址；alternate_score 变为 55，score_address 仍指向 score。
 */
int retarget_pointer_copy(int *target, int *replacement)
{
    if (target == NULL || replacement == NULL) {
        return 0;
    }

    target = replacement;
    *target = 55;
    return 1;
}

/*
 * 用途：观察普通参数副本、指针形参副本重指向、指针修改调用者对象及空指针处理。
 * 参数：无；使用局部 int 和固定新值。
 * 返回值：int；所有输出成功时返回 0，printf 失败或意外状态时返回 1。
 * 异常/错误：C 不使用异常；指针函数拒绝 NULL，printf 的负返回值会使程序返回 1。
 * 副作用：向标准输出写入文本；score 和 alternate_score 由后续指针调用修改。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：启动程序观察 score_address 重指向后仍指向 score，而 alternate_score 变为 55；随后 set_target 将 score 改为 99。
 */
int main(void)
{
    int score = 10;
    int alternate_score = 20;
    int *score_address = &score;
    int changed;

    change_copy(score);
    if (printf("after value copy: %d\n", score) < 0) {
        return 1;
    }

    changed = retarget_pointer_copy(score_address, &alternate_score);
    if (changed != 1 ||
        printf("pointer argument is copied: %s; score=%d; alternate=%d\n",
               score_address == &score ? "yes" : "no",
               score, alternate_score) < 0) {
        return 1;
    }

    changed = set_target(&score, 99);
    if (changed != 1 || printf("after pointer update: %d\n", score) < 0) {
        return 1;
    }

    changed = set_target(NULL, 7);
    if (printf("null pointer rejected: %s\n",
               changed == 0 ? "yes" : "no") < 0) {
        return 1;
    }

    return 0;
}
