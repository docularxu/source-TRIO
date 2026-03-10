# knowledge-map.md - 知识图谱

## 术语表
| 术语 | 定义 |
|------|------|
| driver_ops | 驱动操作契约结构体，通过函数指针实现多态 |
| 静态注册 | 编译期通过全局变量初始化绑定函数指针，运行时不变 |

## 模块关系
```
main.c --extern引用--> usb_ops (usb_driver.c)
main.c & usb_driver.c --include--> driver_api.h (契约定义)
```

## 已确认结论

### C1: main() 初始化调用链（引用：init-flow）
- **调用链：** main() [main.c:7] → usb_ops.init() [main.c:12] → usb_init_internal() [usb_driver.c:11]
- **机制：** 函数指针间接调用，编译期静态绑定（usb_driver.c:27）
- **目标寄存器：** USB_CTRL_REG = 0x4000 + 0x20 = **0x4020**
- **废弃接口 legacy_init_do_not_use() 未被调用**
- 审查状态：✅ Reviewer 通过（Round 2, 2026-03-10）
