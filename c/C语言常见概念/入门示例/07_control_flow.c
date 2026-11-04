/*
 * 文件用途：通过固定数据演示顺序、if、switch、三种循环、break、continue 和 goto。
 * 输入：无；成绩和循环范围由程序固定。
 * 输出：显示选择分支、累加结果及循环与跳转路径。
 * 标准：C99 或更新版本。
 */
#include <stdio.h> /* 引入 printf 的声明，供各控制路径显示结果。 */

/*
 * 用途：按顺序执行条件分支、循环和跳转示例。
 * 参数：无；示例不读取外部数据。
 * 返回值：int；返回 0 表示正常结束。
 * 错误处理：所有循环范围固定且会终止；没有外部输入；printf 写出错误未检查。
 * 副作用：向标准输出写多行文本并更新局部计数器。
 */
int main(void) {
    int score = 88; /* 初始化整数成绩，供 if 和 switch 共同判断。 */
    if (score >= 60) { /* if 在条件非零时进入第一分支；固定成绩 88 满足大于等于 60。 */
        printf("passed = yes\n"); /* 只有成绩达到 60 时输出通过信息。 */
    } else { /* else 表示条件为假时的备选分支，本次固定数据不会进入。 */
        printf("passed = no\n"); /* 成绩小于 60 时才会输出未通过信息。 */
    }
    switch (score / 10) { /* switch 计算整数商 8，并与各 case 常量比较。 */
        case 10: /* 商为 10 时从这里开始，并有意贯穿到下一标签共享 A 级输出。 */
        case 9: /* 商为 9 时直接进入此处；无 break 时也接续执行同一输出。 */
            printf("grade band = A\n"); /* 90 到 109 的示例商值共享此输出。 */
            break; /* break 结束最近的 switch，防止继续执行后续分支。 */
        case 8: /* 固定成绩 88 除以 10 得到 8，因此本次匹配此分支。 */
            printf("grade band = B\n"); /* 输出与当前成绩区间对应的等级。 */
            break; /* 离开 switch，不再落入 default。 */
        default: /* 其他未列出的整数商值进入默认分支。 */
            printf("grade band = C\n"); /* 为未覆盖的商值给出结果。 */
            break; /* 在 default 末尾写 break，明确 switch 结构到此结束。 */
    }
    int sum = 0; /* 初始化累加器，for 循环把符合条件的数加入其中。 */
    for (int number = 1; number <= 5; ++number) { /* 初始化只做一次；每轮先检查不超过 5，循环体后再递增。 */
        if (number == 3) { /* == 比较当前值是否等于 3，不会像 = 那样修改变量。 */
            continue; /* 跳过本轮余下语句；for 仍执行 ++number，再进入下一轮判断。 */
        }
        sum += number; /* 把当前非 3 的数加到 sum，等价于 sum = sum + number。 */
    }
    printf("sum skipping 3 = %d\n", sum); /* 显示 1+2+4+5 的累加结果 12。 */
    int countdown = 2; /* 为 while 循环准备起始值，观察先判断再执行。 */
    while (countdown > 0) { /* 每轮开始先判断条件；条件为假时循环体可以执行零次。 */
        printf("while countdown = %d\n", countdown); /* 显示进入本轮时的当前值。 */
        --countdown; /* 每轮减一，使条件最终变假并结束循环。 */
    }
    int attempts = 0; /* 初始化 do...while 的执行次数计数器。 */
    do { /* do...while 先执行循环体，因此至少执行一次。 */
        ++attempts; /* 在条件判断前记录已经执行的次数。 */
        printf("do-while attempts = %d\n", attempts); /* 显示当前次数，观察首次执行。 */
    } while (attempts < 1); /* 条件为假时结束；末尾分号是 do...while 语法的一部分。 */
    int step = 1; /* 为第二个循环设置起点，演示 break 的退出位置。 */
    while (step <= 5) { /* 条件允许最多走到 5，但循环体会在 3 处提前 break。 */
        if (step == 3) { /* 检查是否到达退出值 3。 */
            printf("break loop at %d\n", step); /* 先显示触发退出的当前值，再执行 break。 */
            break; /* 结束最近一层 while 循环，转到循环之后的语句。 */
        }
        printf("loop step = %d\n", step); /* 只打印没有触发 break 的当前步数。 */
        ++step; /* 每轮递增，保证 while 条件最终能改变。 */
    }
    printf("after break step = %d\n", step); /* 显示退出循环时仍保留的 step 值 3。 */
    if (sum == 12) { /* 固定累加结果为 12，所以本次会执行 goto 分支。 */
        goto finish; /* goto 跳到 main 函数内的 finish 标签，略过下一条提示语句。 */
    }
    printf("sum changed; goto branch skipped\n"); /* 仅当 sum 不等于 12 时才会执行。 */
finish: /* 标签定义同一函数中的跳转位置；标签本身不执行计算。 */
    printf("goto reached finish\n"); /* 本次由 goto 到达此行，显示跳转结果。 */
    return 0; /* 结束 main 并报告正常结束。 */
}

/* 示例输入：无；score 固定为 88，循环范围和起始值也固定。
 * 预期输出：
 * passed = yes
 * grade band = B
 * sum skipping 3 = 12
 * while countdown = 2
 * while countdown = 1
 * do-while attempts = 1
 * loop step = 1
 * loop step = 2
 * break loop at 3
 * after break step = 3
 * goto reached finish
 * 关键结果：if 和 switch 选择路径；for/while 先判断，do...while 先执行；continue 略过本轮，break 离开循环，goto 跳到同一函数的标签。
 */