# lr-core-vector

用 C 语言手写一个 `std::vector`：你要做的事只有一件：根据 `include/vector.h` 的描述**把 `src/vector.c` 里的空壳函数填成能用的实现，让 `make test` 全绿**。

[vector 原理可视化](https://lingrui-studio.github.io/vector-playground/)

本题实现的是只存储 `int` 的教学版动态数组，具体约定以 [include/vector.h](include/vector.h) 为准。

## 目录结构

```
lr-core-vector/
├── readme.md         本文件
├── Makefile          构建脚本（不用改）
├── .gitignore        列举 git 需要忽视的文件
├── .clang-format     格式化要求
├── include/
│   └── vector.h      接口声明 + 函数注释（不用改，但要读懂）
├── src/
│   └── vector.c      ★ 你要实现的地方
└── tests/
    └── test.c        单元测试（不用改）
```

## 自检

补全 [include/vector.c](include/vector.c) 中的函数实现后，项目根目录运行 `make test`，若最后输出结果如下即表示你完成了本项目（本项目只有未完成和已完成两种状态，不存在中间值）：

```bash
== 通过 3393 项，失败 0 项 ==
全部通过，可以 commit & push 了
```

测试始终开启 ASan + UBSan，检测到错误会以失败状态退出，不提供关闭开关。常见错误会被直接指出来，例如：

```
ERROR: AddressSanitizer: heap-buffer-overflow on address 0x... at pc 0x...
READ of size 4 at 0x... thread T0
    #0 0x... in get src/vector.c:52
```

行号会直接指到出问题的那一行，看不懂的把完成代码和报错信息复制给 AI 问一下。

## 提交

- 完成下面的`实现思路`一节，简要说明你的各个函数是如何实现的，尤其注意内存管理的说明
- 把所有修改 commit 并 push 到 GitHub 上自己的 vector 仓库
- 在个人仓库的 Actions 页面手动触发一次自动评分工作流

## 实现思路

* 初始化
-  calloc分配且初始化为0，直接占用了真内存，拖慢速度（元素个数，单个大小）。malloc分配的内存是尚未兑现的。所以选malloc。
- 先确保传入的v不是空指针，否则无法对其初末位置进行定义。
- &v:栈内存，data堆内存，v负责统筹管理，data才是存数据的，数据都存在堆里。
- 初始化三个指针，把随机值取掉。
- malloc从堆申请内存并以"void*"形式返回初始值。用"int*"强制转化并赋值给data。
- cap:data+capacity:边界，end：目前输入的最右边的值，end=cap时需要重新申请内存。
- 注意最后一步是+capcity而不是`+capcity*sizeof(int)`,因为系统默认乘了。

* 释放
- free了内存之后，需要重新置空，避免泄露。
- return不用带数据，因为数据类型为void，不返回值。return表停止执行。

* 求大小
- NULL的地址为0x0,处于用户空间最低端，被系统设定为不可读写，所以空指针不能加减，需要判data是否非空，data不是空指针说明cap和end都非空
- 结构体vector的作用？如果没有，需要对data等的二级指针进行赋值修改。（一层存数据一层地址）

* 扩容
- malloc分配全新内存，（总字节数size）。如果用它申请需要把旧仓库里的东西挨个搬到新地址再free旧的。
- realloc调整已有大小，可以原地扩容，如果超了就自己迁移好并自动free掉旧地址，（旧指针，新字节数:最后扩完的总大小）。
- 倍增扩容：每次申请`capcity*2`个空间。
- 不要覆盖原指针，因为realloc一旦申请失败就会置空data丢失原来的地址。

* pop_back
- `v->end`指向最后一个有效元素的下一个位置。
- 使用`v->end--;`逻辑删除，数据还在堆内存但已经不属于vector,可以被下一次push进来的新数据覆盖。

* reserve
- 使用realloc实现扩容，先计算实际容量，判断需不需要扩容，同时限定目标容量不超出储存范围。
- 如果data不存在说明没有存数据，则size为0。
- 为了避免申请失败丢掉原地址用new_data来承接。

* shrink
- 如果data为空则size已经=capacity，直接`return 0`。
- realloc既可以用于扩容也可以用于缩容，一般不会发生迁移。
- 将`data=NULL`,`data=end`,`capacity=size`的情况单独讨论。
- 对于其他情况修改内存并重新调整指针位置。

*clear
- 不动容量只用把尾指针移到首指针的位置。