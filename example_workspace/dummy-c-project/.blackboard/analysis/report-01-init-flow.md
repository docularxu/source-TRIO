# Report 01: init-flow — main() 初始化调用链追踪

## 概述

`main()` 通过全局结构体 `usb_ops` 的函数指针 `.init` 间接调用 `usb_init_internal()`，完成 USB 控制寄存器（地址 `0x4020`）的初始化。

## 调用链

```
main()                              [main.c:7]
  └─ usb_ops.init()                 [main.c:12]  — 函数指针间接调用
       │
       │  绑定关系：usb_ops.init = usb_init_internal  [usb_driver.c:27]
       │
       └─ usb_init_internal()       [usb_driver.c:11]
            └─ printf(... USB_CTRL_REG)  [usb_driver.c:12]
```

## 关键数据结构

### `struct driver_ops` [driver_api.h:8-11]

```c
struct driver_ops {
    int (*init)(void);
    void (*shutdown)(void);
};
```

驱动契约结构体，定义 `init` 和 `shutdown` 两个函数指针槽位。

### `usb_ops` [usb_driver.c:26-29]

```c
struct driver_ops usb_ops = {
    .init = usb_init_internal,
    .shutdown = NULL
};
```

全局变量，编译期静态绑定。`main.c:5` 通过 `extern struct driver_ops usb_ops` 引用。

## 关键逻辑

### 函数指针间接调用

`main.c:12` 执行 `usb_ops.init()`。这不是直接函数调用，而是通过 `struct driver_ops` 的 `.init` 函数指针进行间接调用。

**绑定证据：** `usb_driver.c:27` 将 `.init` 赋值为 `usb_init_internal`，该赋值发生在编译期（全局变量初始化），运行时不会改变。

### 寄存器地址计算

`usb_init_internal()` 操作的目标寄存器地址由三个宏定义计算得出：

| 宏 | 值 | 定义位置 |
|---|---|---|
| `BASE_ADDR` | `0x4000` | usb_driver.c:6 |
| `USB_OFFSET` | `0x20` | usb_driver.c:7 |
| `USB_CTRL_REG` | `BASE_ADDR + USB_OFFSET` | usb_driver.c:8 |

**计算过程：** `USB_CTRL_REG = 0x4000 + 0x20 = 0x4020`

`usb_init_internal()` 在 `usb_driver.c:12` 使用 `USB_CTRL_REG`（即 `0x4020`）作为 USB 控制寄存器地址。

## 动态分发 / 隐性跳转

| 跳转点 | 变量类型 | 赋值位置 | 实际目标 |
|--------|---------|---------|---------|
| `usb_ops.init()` [main.c:12] | `int (*init)(void)` (in `struct driver_ops`) | usb_driver.c:27 | `usb_init_internal` [usb_driver.c:11] |

该跳转为编译期静态绑定，无运行时动态分发。

## 废弃接口排除

`legacy_init_do_not_use()` [usb_driver.c:17] 在整个初始化链中未被调用。`usb_ops.init` 绑定的是 `usb_init_internal`，与 `legacy_init_do_not_use` 无关。`driver_api.h:14` 对其的声明仅为历史遗留。

## 与其他模块的关系

- **main.c** → 通过 `extern` 引用 `usb_ops`（usb_driver.c 提供）
- **driver_api.h** → 定义契约结构体 `struct driver_ops`，被 main.c 和 usb_driver.c 共同包含
- **usb_driver.c** → 提供 `usb_ops` 实例和 `usb_init_internal` 实现

## 待分析

无。本任务所有跳转和地址均已完全解析。
