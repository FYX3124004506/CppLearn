/*
 * 文件职责：声明多个翻译单元共享的函数和外部对象。
 * 输入/输出：不单独运行；由实现单元与调用方包含。
 * 构建方式：将对应的实现源文件和调用方源文件一起编译并链接。
 */
#ifndef FUNCTION_PUBLIC_INTERFACE_H
#define FUNCTION_PUBLIC_INTERFACE_H

/*
 * 跨翻译单元共享的调用计数。
 * 在一个实现单元中定义并初始化；每次调用 next_sequence 后增加 1。
 */
extern unsigned int shared_call_count;

/*
 * 用途：计算两个输入值之和，再加上模块私有的固定调整量。
 * 参数：left、right 的有效范围均为 [-1000, 1000]。
 * 返回值：left + right + 5。
 * 异常/错误：C 不使用异常；不检查范围，调用者必须遵守参数约定。
 * 副作用：无；不会改变 shared_call_count。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：add_with_adjustment(6, 5) 返回 16。
 */
int add_with_adjustment(int left, int right);

/*
 * 用途：取得本模块连续调用的序号。
 * 参数：无；void 原型明确表示不接收参数。
 * 返回值：从 1 开始递增的 unsigned int 序号；到达类型上限后按无符号规则回绕。
 * 异常/错误：C 不使用异常；没有错误返回码。
 * 副作用：更新模块内的静态局部序号，并增加 shared_call_count。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：顺序调用两次通常分别得到 1 和 2；本示例在发生回绕前观察这两个值。
 */
unsigned int next_sequence(void);

#endif
