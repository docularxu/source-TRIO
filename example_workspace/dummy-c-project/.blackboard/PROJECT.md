# PROJECT.md - dummy-c-project

## 概览
模拟嵌入式系统启动流程的 dummy 项目，用于测试 Multi-Agent 代码分析框架。

## 代码路径
`~/.openclaw/workspace/source-driven-agent-framework/example_workspace/dummy-c-project/`

## 文件清单
| 文件 | 用途 |
|------|------|
| main.c | 入口，调用驱动初始化 |
| driver_api.h | 全局契约结构体 `driver_ops` 定义 |
| usb_driver.c | USB 驱动实现，含寄存器地址定义 |
| README.md | 项目说明 |

## 分析目标
查清 `main()` 中初始化流程实际操作的硬件寄存器地址。

## 模块拆分
本项目体量小，拆分为 1 个分析任务：
1. **init-flow** — main() 初始化调用链追踪（含寄存器地址解析）

## 模块依赖
无跨模块依赖（单任务）。

## Commit Hash
N/A（本地项目，无 git 管理）
