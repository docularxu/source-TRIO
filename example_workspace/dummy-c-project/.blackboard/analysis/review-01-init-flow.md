# Review 01: init-flow

## Round 1 (2026-03-10 08:51 EDT)

结论：❌ 打回

### 代码引用抽查（抽查了 13/13 条，100%）

**main.c — 全部正确：**

- ✅ `main()` @ main.c:7
  验证: `$ sed -n '7p' main.c`
  输出: `int main() {`  ← 匹配
- ✅ `usb_ops.init()` @ main.c:12
  验证: `$ sed -n '12p' main.c`
  输出: `    int ret = usb_ops.init();`  ← 匹配
- ✅ `extern struct driver_ops usb_ops` @ main.c:5
  验证: `$ sed -n '5p' main.c`
  输出: `extern struct driver_ops usb_ops;`  ← 匹配

**driver_api.h — 部分偏移：**

- ❌ `struct driver_ops` @ driver_api.h:9-12（报告声称）
  验证: `$ cat -n driver_api.h`
  实际: driver_api.h:**8-11**（`struct driver_ops {` 在第8行，`};` 在第11行）
  偏移 1 行
- ✅ `legacy_init_do_not_use()` 声明 @ driver_api.h:14 — 匹配

**usb_driver.c — 大面积行号错误（全部偏移 1-4 行）：**

- ❌ `BASE_ADDR` @ usb_driver.c:7（报告声称）
  验证: `$ sed -n '6p' usb_driver.c`
  输出: `#define BASE_ADDR   0x4000`
  实际在第 **6** 行，报告说第 7 行 ← 偏移 +1
- ❌ `USB_OFFSET` @ usb_driver.c:8（报告声称）
  验证: `$ sed -n '7p' usb_driver.c`
  输出: `#define USB_OFFSET  0x20`
  实际在第 **7** 行 ← 偏移 +1
- ❌ `USB_CTRL_REG` @ usb_driver.c:9（报告声称）
  验证: `$ sed -n '8p' usb_driver.c`
  输出: `#define USB_CTRL_REG (BASE_ADDR + USB_OFFSET)`
  实际在第 **8** 行 ← 偏移 +1
- ❌ `usb_init_internal()` 定义 @ usb_driver.c:13（报告声称）
  验证: `$ sed -n '11p' usb_driver.c`
  输出: `static int usb_init_internal(void) {`
  实际在第 **11** 行 ← 偏移 +2
- ❌ printf(USB_CTRL_REG) @ usb_driver.c:14（报告声称）
  验证: `$ sed -n '12p' usb_driver.c`
  输出: `    printf("Initializing USB at Register: 0x%X\n", USB_CTRL_REG);`
  实际在第 **12** 行 ← 偏移 +2
- ❌ `legacy_init_do_not_use()` @ usb_driver.c:19（报告声称）
  验证: `$ sed -n '17p' usb_driver.c`
  输出: `void legacy_init_do_not_use(void) {`
  实际在第 **17** 行 ← 偏移 +2
- ❌ `usb_ops` 定义 @ usb_driver.c:30（报告声称）
  验证: `$ sed -n '26p' usb_driver.c`
  输出: `struct driver_ops usb_ops = {`
  实际在第 **26** 行 ← 偏移 +4
- ❌ `.init = usb_init_internal` @ usb_driver.c:31（报告声称）
  验证: `$ sed -n '27p' usb_driver.c`
  输出: `    .init = usb_init_internal,`
  实际在第 **27** 行 ← 偏移 +4

**SYMBOL_INDEX.md 同步错误：**

- ❌ `usb_init_internal` 标注 usb_driver.c:13 → 实际 usb_driver.c:**11**
- ❌ `usb_ops` 标注 usb_driver.c:30 → 实际 usb_driver.c:**26**
- ❌ `BASE_ADDR` 标注 usb_driver.c:7 → 实际 usb_driver.c:**6**
- ❌ `USB_OFFSET` 标注 usb_driver.c:8 → 实际 usb_driver.c:**7**
- ❌ `USB_CTRL_REG` 标注 usb_driver.c:9 → 实际 usb_driver.c:**8**

### 独立追踪

从 `main()` 入口独立追踪 3 层：

1. **main.c:12** `int ret = usb_ops.init();` — 函数指针调用，需要找 `usb_ops` 的定义
2. **main.c:5** `extern struct driver_ops usb_ops;` — 外部链接，搜索 `usb_ops` 的实际定义
3. `$ grep -n 'usb_ops' usb_driver.c` → 第 26 行: `struct driver_ops usb_ops = {`，第 27 行: `.init = usb_init_internal`
4. `usb_init_internal` 定义在 usb_driver.c:11，函数体是第 12 行的 printf 和第 13 行的 return 0

**独立追踪结论：** 调用链 `main() → usb_ops.init() → usb_init_internal()` 正确。寄存器地址 `0x4000 + 0x20 = 0x4020` 计算正确。逻辑正确，但 Researcher 的行号引用系统性偏移。

### 语义正确性

- ✅ 调用链逻辑正确：`main()` → `usb_ops.init()` → `usb_init_internal()`
- ✅ 函数指针间接调用机制描述正确
- ✅ 寄存器地址 0x4020 = 0x4000 + 0x20 计算正确
- ✅ `legacy_init_do_not_use()` 正确排除
- ✅ 编译期静态绑定的判断正确
- ✅ `usb_init_internal` 为 `static` 函数，报告正确识别
- ✅ 错误处理路径覆盖（main.c:14-18 的 ret 检查）

### 问题清单

- 🔴 P1: **usb_driver.c 行号大面积错误** — 报告中所有 usb_driver.c 的行号引用均偏移 1-4 行。13 条引用中 8 条行号错误（错误率 62%）。这属于"幻觉引用"，违反代码引用准确性要求。任何依赖这些行号进行交叉验证的下游消费者都会被误导。
- 🔴 P1: **SYMBOL_INDEX.md 行号与源码不一致** — 5 个符号的行号标注错误，与报告中的错误同源。SYMBOL_INDEX 作为全局参考索引，行号错误会传播到后续所有任务。
- 🟡 P2: **driver_api.h 结构体行号偏移** — `struct driver_ops` 报告标注 9-12 行，实际为 8-11 行，偏移 1 行。

### 修复要求

1. 用 CLI 工具（`cat -n` 或 `sed -n`）重新确认 usb_driver.c 中所有符号的实际行号，修正报告中的全部引用
2. 同步修正 SYMBOL_INDEX.md 中对应符号的行号
3. 修正 driver_api.h 中 `struct driver_ops` 的行号范围为 8-11

---

## Round 2 (2026-03-10 08:54 EDT)

结论：✅ 通过

### 代码引用抽查（抽查了 13/13 条，100%）

**main.c：**
- ✅ `main()` @ main.c:7 → `int main() {`
- ✅ `usb_ops.init()` @ main.c:12 → `int ret = usb_ops.init();`
- ✅ `extern` @ main.c:5 → `extern struct driver_ops usb_ops;`

**usb_driver.c：**
- ✅ `BASE_ADDR` @ usb_driver.c:6 → `#define BASE_ADDR   0x4000`
- ✅ `USB_OFFSET` @ usb_driver.c:7 → `#define USB_OFFSET  0x20`
- ✅ `USB_CTRL_REG` @ usb_driver.c:8 → `#define USB_CTRL_REG (BASE_ADDR + USB_OFFSET)`
- ✅ `usb_init_internal()` @ usb_driver.c:11 → `static int usb_init_internal(void) {`
- ✅ printf @ usb_driver.c:12 → `printf("Initializing USB at Register: 0x%X\n", USB_CTRL_REG);`
- ✅ `legacy_init_do_not_use()` @ usb_driver.c:17 → `void legacy_init_do_not_use(void) {`
- ✅ `usb_ops` @ usb_driver.c:26-29 → 完整结构体初始化块
- ✅ `.init = usb_init_internal` @ usb_driver.c:27 → `.init = usb_init_internal,`

**driver_api.h：**
- ✅ `struct driver_ops` @ driver_api.h:8-11 → 完整结构体定义
- ✅ `legacy_init_do_not_use` 声明 @ driver_api.h:14 → `void legacy_init_do_not_use(void);`

### SYMBOL_INDEX.md 验证

所有 7 个符号行号已与源码同步，逐条确认无误：
- ✅ `struct driver_ops` → driver_api.h:8
- ✅ `usb_ops` → usb_driver.c:26
- ✅ `usb_init_internal` → usb_driver.c:11
- ✅ `legacy_init_do_not_use` → usb_driver.c:17
- ✅ `BASE_ADDR` → usb_driver.c:6
- ✅ `USB_OFFSET` → usb_driver.c:7
- ✅ `USB_CTRL_REG` → usb_driver.c:8

### Round 1 问题状态

- 🔴 P1 usb_driver.c 行号错误 → ✅ 已修正，全部验证通过
- 🔴 P1 SYMBOL_INDEX.md 行号错误 → ✅ 已修正，全部验证通过
- 🟡 P2 driver_api.h 结构体行号偏移 → ✅ 已修正为 8-11

### 结论

所有 Round 1 提出的问题均已修正。13 条代码引用 100% 与物理源码匹配，SYMBOL_INDEX 同步完成。语义分析（调用链、函数指针绑定、寄存器地址计算）在 Round 1 已确认正确。

**报告通过审查。**
