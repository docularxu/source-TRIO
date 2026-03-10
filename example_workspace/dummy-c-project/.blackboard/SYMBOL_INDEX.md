# SYMBOL_INDEX.md - 全局符号表

| 符号 | 类型 | 定义位置 | 备注 |
|------|------|----------|------|
| `struct driver_ops` | 结构体 | driver_api.h:8 | 全局驱动契约，含 init/shutdown 函数指针 |
| `usb_ops` | 全局变量 | usb_driver.c:26 | driver_ops 实例，.init = usb_init_internal |
| `usb_init_internal` | 静态函数 | usb_driver.c:11 | USB 实际初始化逻辑 |
| `legacy_init_do_not_use` | 函数 | usb_driver.c:17 | 废弃接口，不应被调用 |
| `BASE_ADDR` | 宏 | usb_driver.c:6 | 0x4000 |
| `USB_OFFSET` | 宏 | usb_driver.c:7 | 0x20 |
| `USB_CTRL_REG` | 宏 | usb_driver.c:8 | BASE_ADDR + USB_OFFSET |
