# REPORT.md - dummy-c-project 代码分析最终报告

**日期：** 2026-03-10
**分析目标：** 查清 main() 初始化流程实际操作的硬件寄存器地址

---

## 1. 核心结论

`main()` 通过函数指针间接调用 `usb_init_internal()`，操作的硬件寄存器地址为 **0x4020**。

## 2. 完整调用链

```
main()                              [main.c:7]
  └─ usb_ops.init()                 [main.c:12]    ← 函数指针间接调用
       │
       │  绑定: usb_ops.init = usb_init_internal   [usb_driver.c:27]
       │  方式: 编译期静态绑定（全局变量初始化）
       │
       └─ usb_init_internal()       [usb_driver.c:11]
            └─ 操作 USB_CTRL_REG    [usb_driver.c:12]
```

## 3. 寄存器地址计算

| 宏 | 值 | 定义位置 |
|---|---|---|
| BASE_ADDR | 0x4000 | usb_driver.c:6 |
| USB_OFFSET | 0x20 | usb_driver.c:7 |
| USB_CTRL_REG | BASE_ADDR + USB_OFFSET | usb_driver.c:8 |

**USB_CTRL_REG = 0x4000 + 0x20 = 0x4020**

## 4. 关键设计模式

- **函数指针多态：** `struct driver_ops`（driver_api.h:8-11）定义 init/shutdown 函数指针槽位，`usb_ops`（usb_driver.c:26）为其实例
- **静态注册：** 函数指针在编译期绑定，无运行时动态分发
- **废弃接口：** `legacy_init_do_not_use()` 存在于代码中但未被调用

## 5. 分析质量记录

| 指标 | 结果 |
|------|------|
| 审查轮次 | 2 轮 |
| Round 1 | ❌ 打回（行号偏移，错误率 62%） |
| Round 2 | ✅ 通过（13/13 引用验证正确） |
| 语义正确性 | Round 1 即确认正确 |
| 核心教训 | Researcher 必须用 CLI 工具验证行号，不可凭推断 |

---

**Sign-off：** ✅ Guodong 确认（2026-03-10）
