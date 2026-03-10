# init-flow - 分析报告

## 概述
DDR 初始化主入口，通过函数指针表调用平台特定实现。

## 调用链
```
main()                              # main.c:11
  ├─ get_ddr_ops()                  # main.c:15  [__weak, see 动态分发]
  ├─ ops->init(&g_ddr_cfg)          # main.c:22  [函数指针, see 动态分发]
  └─ ops->train(&g_ddr_cfg)         # main.c:28  [#ifdef CONFIG_DDR_TRAINING]
```

## 关键数据结构
```c
struct ddr_config {                 // ddr.h:13
    unsigned int frequency;         // MHz, 默认值由 DDR_FREQ_DEFAULT 决定
    int timing_mode;                // TIMING_AUTO(0) 或 TIMING_MANUAL(1)
    unsigned int rank_count;        // 由平台 init 填充
    void *platform_data;            // 平台私有数据，本项目未使用
};
```
`g_ddr_cfg` 是全局变量（main.c:8），在 main() 中初始化 frequency 和 timing_mode，传递给所有后续调用。

## 关键逻辑
1. main.c:16-18: 检查 ops 和必要函数指针非空，否则返回 -1
2. main.c:20-21: 设置默认频率和 timing 模式
3. main.c:28: `#ifdef CONFIG_DDR_TRAINING` 控制 training 阶段，未定义则跳过

## 动态分发/隐性跳转
| 跳转点 | 变量类型 | 赋值位置 | 实际目标 |
|--------|----------|----------|----------|
| get_ddr_ops() | __weak 函数 | main.c:37 (弱定义) | platform_k1.c:19 (强覆盖) |
| ops->init | struct ddr_ops.init | platform_k1.c:13 (.init = k1_ddr_init) | platform_k1.c:23: k1_ddr_init() |
| ops->train | struct ddr_ops.train | platform_k1.c:14 (.train = k1_ddr_train) | platform_k1.c:43: k1_ddr_train() |

## 与其他模块的关系
- 下游: platform-ops（提供 ops 实现）, training（ops->train 调用）
- 共享: g_ddr_cfg 全局变量

## 待分析
- 无
