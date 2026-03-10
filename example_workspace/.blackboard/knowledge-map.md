# 知识图谱 - DDR Demo Project

## 模块关系图
```mermaid
graph TD
  MAIN[main.c: main()] -->|"calls ops->init()"| INIT[platform_k1.c: k1_ddr_init()]
  MAIN -->|"calls ops->train()"| TRAIN[platform_k1.c: k1_ddr_train()]
  MAIN -->|"__weak override"| GETOPS[platform_k1.c: get_ddr_ops()]
  GETOPS -->|"returns &k1_ops"| OPS["k1_ops{.init, .train}"]
  OPS -->|".init"| INIT
  OPS -->|".train"| TRAIN
```

## 已确认结论

### 模块 01: init-flow (analysis/01-init-flow.md)
- [C1] main() 通过函数指针 ops->init() 调用平台实现（main.c:22）→ 被引用: 模块02
- [C2] g_ddr_cfg 是全局共享状态，贯穿整个初始化链路（main.c:8）→ 被引用: 模块02, 模块03
- [C3] CONFIG_DDR_TRAINING 控制 training 阶段是否执行（main.c:28）→ 被引用: 模块03

### 模块 02: platform-ops (analysis/02-platform-ops.md)
- [C4] get_ddr_ops() 在 main.c:37 为 __weak，被 platform_k1.c:19 覆盖 → 被引用: 无
- [C5] k1_ops 静态绑定 .init=k1_ddr_init, .train=k1_ddr_train（platform_k1.c:12-14）→ 被引用: 模块03
- [C6] CONFIG_DDR_HIGH_SPEED 分出两条 PHY 配置路径（platform_k1.c:30-36）→ 被引用: 无

### 模块 03: training (analysis/03-training.md)
- [C7] training 仅在 TIMING_AUTO 模式下执行自动序列（platform_k1.c:46）→ 被引用: 无
- [C8] training 通过 spin wait 等待硬件完成，无超时保护（platform_k1.c:51-52）→ 被引用: 无

## 开放问题
- [x] ~~ops->shutdown 未实现（platform_k1.c:15）~~ 确认为 TODO，非阻塞

## 术语表
- PHY: DDR Physical Layer Interface
- ops: 函数指针操作表 (struct ddr_ops)
- __weak: GCC 弱符号属性，允许被同名强符号覆盖
