# PROTOCOL.md - Source-TRIO 流程协议

> 本文档是 source-TRIO 团队的权威流程协议。
> 所有 agent 必须遵守本文档，与 SOUL.md 冲突时以本文档为准。

## 1. 核心原则

**No Evidence, No Claim**: 每条分析结论必须附带 `文件:行号` 引用。无引用的结论禁止出现在分析文档中。

## 2. 团队角色

| 角色 | 核心职责 |
|------|---------|
| ⚡ Jarvis-Arch | 模块拆分、状态机驱动、知识图谱整合、写 task.md、spawn Researcher/Reviewer |
| 🔬 Researcher | 读代码、追调用链、输出分析文档、更新 tasks-backlog.md 状态 |
| 🔍 Reviewer | CLI 验证代码引用、独立追踪调用链、写 review.md |

## 3. 三阶段流程

### Phase 1: 侦察与规划
1. **目标对齐**: 与老板反复对话，澄清分析目标和边界。必须确认：分析范围、关注点、已知入口、**目标编译配置**（defconfig / 关键 CONFIG 选项，决定 #ifdef 分支走向）
2. **项目初始化与代码侦察**: 从 `shared/templates/` 复制模板到项目 `.blackboard/` 作为起点。侦察代码库（目录结构、构建系统、入口点），在模板基础上填写 PROJECT.md + SYMBOL_INDEX.md + tasks-backlog.md + knowledge-map.md 初版。模板定义必填字段（编译配置、代码库信息等），Jarvis-Arch 可根据项目增删其他字段，但必填字段不能跳过
3. **计划审查**: Reviewer 审查分析计划
4. **Sign-off**: 老板确认后进入 Phase 2

⚠️ **Sign-off 是硬性门禁。** 老板未确认前，禁止将任何任务从 [TODO] 改为 [Analyzing]。

⚠️ **编译配置是分析前提，不是可选项。** C 代码库中大量逻辑隐藏在 `#ifdef` 分支里。如果不在 Phase 1 就锁定目标编译配置，Researcher 和 Reviewer 可能在不同的 `#ifdef` 分支上工作，导致分析结论互相矛盾却都"正确"。Jarvis-Arch 必须在 Step 1 与人类工程师确认 defconfig 和关键 CONFIG 选项，记录到 PROJECT.md，作为后续所有分析和验证的共同基准。

### Phase 2: 逐模块深度分析

**⚠️ 严格串行。同一时刻 tasks-backlog.md 中最多一个 [Analyzing]。禁止并行。**

每个模块的完整循环:
1. Jarvis-Arch 将模块从 [TODO] 改为 [Analyzing]，写 task.md
2. Jarvis-Arch spawn Researcher
3. Researcher 读 PROTOCOL.md + PROJECT.md + knowledge-map.md + SYMBOL_INDEX.md + task.md
4. Researcher 分析代码，输出 analysis/NN-module.md
5. Researcher 自检所有 `文件:行号` 引用（用 `cat -n` 或 `sed -n`）
6. Researcher 将状态改为 [Reviewing]
7. Jarvis-Arch spawn Reviewer
8. Reviewer 用 CLI 命令验证代码引用（≥30% 抽查），独立追踪至少一条调用链，写 review.md
9. 通过 → Jarvis-Arch 更新 knowledge-map.md，标记 [Done]，驱动复盘，进入下一个 [TODO]
10. 不通过 → Researcher 修正，最多 3 轮。3 轮未过标记 [Escalated]，老板介入

**一个模块走完整个 1-10 循环后，才能开始下一个模块。**

**调用链追踪深度限制：** Researcher 追踪调用链超过 task.md 指定的层数（默认 5 层）仍未到达边界时，必须暂停并向 Jarvis-Arch 汇报当前位置和困难，由 Jarvis-Arch 决定是继续深追、缩小范围、还是标记 [PENDING] 跳过。禁止无限追踪。

### Phase 3: 整合报告
- Jarvis-Arch 基于所有 analysis/*.md + knowledge-map.md + retro.md 整合 REPORT.md
- 老板 sign-off 后项目结束

### 各阶段输入/输出/完成标志

**Phase 1:**
- 📥 输入：老板的初始分析目标（可能模糊）+ 代码库访问方式（本地 clone 路径 / GitHub URL / commit hash）
- 📤 输出：PROJECT.md（代码库信息、目录结构、模块拆分、分析计划、模块依赖顺序、commit hash）+ tasks-backlog.md（全部 [TODO]）+ knowledge-map.md 初版（粗粒度模块关系 + 术语表）+ SYMBOL_INDEX.md（全局符号表）
- ✅ 完成标志：Reviewer 通过 + 老板 sign-off

**Phase 2（每个模块一个循环）:**
- 📥 输入：task.md + knowledge-map.md + PROJECT.md + SYMBOL_INDEX.md + 源码
- 📤 输出：analysis/NN-module.md + review.md + knowledge-map.md 更新（新增已确认结论 + 下游引用关系）+ tasks-backlog.md 更新（→ [Done]）+ retro.md 本模块复盘
- ✅ 完成标志：Reviewer 通过（≤3 轮），Jarvis-Arch 已整合进知识图谱

**Phase 3:**
- 📥 输入：所有 analysis/*.md + knowledge-map.md（最终版）+ retro.md
- 📤 输出：REPORT.md（最终整合报告）
- ✅ 完成标志：老板 sign-off

## 4. 目录结构

```
# Agent 的"家" - 不随项目变
~/.openclaw/agents/jarvis-arch/
  SOUL.md              # 身份与性格
  AGENTS.md            # 启动指令（强制读 PROTOCOL.md）
  PROTOCOL.md          # → symlink → shared/PROTOCOL.md
  guard.sh             # → symlink → shared/guard.sh
  MEMORY.md            # 长期记忆（跨项目经验）
  memory/

~/.openclaw/agents/researcher/
  （同上结构）

~/.openclaw/agents/reviewer/
  （同上结构）

~/.openclaw/agents/shared/
  PROTOCOL.md          # 流程协议（本文档）
  guard.sh             # 状态变更合规检查脚本

# 项目目录 - 换项目换这里
~/projects/<project-name>/
  PROJECT.md           # 项目概览：代码库信息、目录结构、模块拆分、分析计划、commit hash
  REPORT.md            # 最终整合报告（Phase 3 产出）
  .blackboard/         # 共享黑板
    tasks-backlog.md   # 模块分析任务列表 + 状态（唯一进度 source of truth）
    task.md            # 当前分析任务（入口点、范围、关注点）
    review.md          # 审查意见（按轮次追加）
    knowledge-map.md   # 全局知识图谱（模块关系、已确认结论、开放问题）
    SYMBOL_INDEX.md    # 全局符号表（Phase 1 由 Jarvis-Arch 用 ctags/cscope 生成）
    decisions.md       # 分析决策记录（"为什么选择这个分析路径"）
    retro.md           # 复盘记录
    archive/           # 历史归档
  analysis/            # 分析文档
    01-module-name.md
    02-module-name.md
    ...
```

**关键说明：**
- PROTOCOL.md 跟团队走（symlink 共享），不跟项目走。改一个文件三个 agent 同时生效。
- PROJECT.md / .blackboard/* 跟项目走，每次新项目由 Jarvis-Arch 创建。
- 代码库本身不在项目目录内 - PROJECT.md 中记录代码库路径。
- SYMBOL_INDEX.md：Phase 1 由 Jarvis-Arch 使用 ctags/cscope 或 LSP 生成，记录关键全局结构体、核心宏、全局变量的定义位置。降低 Researcher 检索成本和幻觉率。Jarvis-Arch 在整合每个模块时按需补充新符号。
- decisions.md：记录架构级别的"为什么"（如"为什么跳过 drivers/usb？""为什么判定 xxx 为死代码？"），确保分析过程的不可辩驳性。

## 5. 状态机

tasks-backlog.md 中每个模块的状态标签：

- ⏳ `[TODO]` - 待分配
- 🏃 `[Analyzing]` - Jarvis-Arch 已写 task.md，Researcher 正在处理
- 🔍 `[Reviewing]` - Researcher 已输出分析文档，Reviewer 正在核查
- ✅ `[Done]` - Reviewer 通过，Jarvis-Arch 已更新知识图谱
- ⚠️ `[Needs Re-evaluation]` - 底层结论变更，退回重审（优先级高于新 [TODO]）
- 🔒 `[Escalated]` - 超过 3 轮未决，老板介入

**[Escalated] 恢复机制**：老板介入后，通过更新 task.md（补充说明、缩小范围）或写 decisions.md（裁决争议点）来提供指导。Jarvis-Arch 收到老板指示后，将状态从 [Escalated] 改回 [Analyzing] 或 [TODO]，流程恢复。老板未明确指示前，[Escalated] 状态不得被任何 agent 修改。

### 强校验执行闭环

1. Jarvis-Arch 检查 backlog，看到 [TODO] → 改为 [Analyzing]，写 task.md，spawn Researcher
2. Researcher 分析完 → 生成分析文档，将状态改为 [Reviewing]
3. Jarvis-Arch 看到 [Reviewing] → spawn Reviewer
4. 争论写在 review.md 里，超过 3 轮 → [Escalated]
5. 断点续传：系统重启后，Jarvis-Arch 看 tasks-backlog.md 状态即可恢复

### 串行规则（硬约束）

**在变更 tasks-backlog.md 状态前，必须运行 `./guard.sh <action>`。返回非 OK 则禁止操作。**

```
./guard.sh start-analyzing    # 检查是否已有 [Analyzing]
./guard.sh start-reviewing     # 检查分析文档是否存在
```

## 6. 通信时序

### Phase 1
```
老板 → Jarvis-Arch: 分析目标
Jarvis-Arch ↔ 老板: 反复对话澄清
Jarvis-Arch → 黑板: 写 PROJECT.md + tasks-backlog.md
Jarvis-Arch → Reviewer: "请审查分析计划"
Reviewer → Jarvis-Arch: 通过/有问题（≤3 轮）
Jarvis-Arch → 老板: 请 sign-off
老板 → Jarvis-Arch: 确认
```

### Phase 2（每个模块）
```
Jarvis-Arch → 黑板: 写 task.md，改状态 [Analyzing]
Jarvis-Arch → Researcher: "分析模块 X，看 task.md"
Researcher → 黑板: 读 PROJECT.md + knowledge-map.md + task.md
Researcher → 黑板: 写 analysis/xx-module.md，改状态 [Reviewing]
Researcher → Reviewer: "分析完成，请审查"
Reviewer → 黑板: 读 task.md + analysis/xx-module.md，用 CLI 验证
Reviewer → 黑板: 写 review.md
Reviewer → Researcher: "有问题，看 review.md" / "通过"
（≤3 轮，超过升级老板）
Reviewer → Jarvis-Arch: "review 通过"
Jarvis-Arch → 黑板: 更新 knowledge-map.md，改状态 [Done]
Jarvis-Arch → 老板: 汇报进展
```

## 7. task.md 模板

```
# 模块名称
## 分析目标（这个模块干什么、为什么分析它）
## 入口点（从哪个函数/文件开始追、已知的调用者）
## 分析范围（追到哪里停、哪些分支不用追）
## 关注点（数据结构？寄存器配置？开源/闭源边界？）
## 动态分发/隐性跳转（已知的函数指针、ops、宏、__weak）
## 上下文（依赖前序模块的哪些结论）
## 产出要求（调用链图、数据结构说明、待分析清单）
```

## 8. 分析文档产出格式

Researcher 输出的分析文档必须包含以下结构（缺一不可）：

```
# 模块名称 - 分析报告
## 概述（一句话说明模块功能）
## 调用链（缩进树形，每个节点附 文件:行号）
## 关键数据结构（结构体定义、字段含义、传递路径）
## 关键逻辑（条件分支、配置解析、硬件交互）
## 动态分发/隐性跳转（函数指针绑定、宏展开、__weak。未解析标 [UNRESOLVED]）
## 开源/闭源边界（如适用）
## 与其他模块的关系（上游调用者、下游依赖、共享全局变量）
## 待分析（看不懂的、需要硬件手册的）
```

引用格式：`文件路径:行号`（相对于代码库根目录），必须可通过 `sed -n 'Np' file` 验证。

## 9. review.md 格式

按轮次追加，不覆盖历史。

**轮次计数器（必须）**：LLM 无状态，无法自行记忆当前是第几轮 review。Reviewer 每次驳回必须在标题中写明显式轮次 `[REJECT, Round X/3]`。Researcher 修正后回复时也必须带轮次号。这是 3 轮熔断机制能被正确执行的前提。

```
## Round N (日期时间) [REJECT, Round N/3]
结论：❌ 打回 / ✅ 通过

### 代码引用抽查（抽查了 X/Y 条）
- ✅ func() @ file:line
  验证: $ sed -n 'Np' file
  输出: (实际代码行)    ← 匹配
- ❌ func() @ file:line
  验证: $ sed -n 'Np' file
  输出: (实际代码行)    ← 不匹配！

### 独立追踪
（从哪个入口独立追了几层，发现了什么）

### 问题清单
- 🔴 P1: ...
- 🟡 P2: ...
```

## 10. Reviewer 审查规则

### 审查清单
| 检查项 | 方法 | 严重程度 |
|--------|------|---------|
| 代码引用准确性 | CLI 验证 ≥30% 引用 | 🔴 P1 |
| 调用链完整性 | 检查遗漏分支、#ifdef、错误处理 | 🔴 P1 |
| 结论有据 | No Evidence, No Claim | 🔴 P1 |
| 隐性跳转处理 | 函数指针/ops/宏/\_\_weak 是否解析 | 🔴 P1 |
| 数据流准确性 | 追踪关键参数传递路径 | 🟡 P2 |
| 待分析标注 | 是否诚实标注不懂的部分 | 🟡 P2 |
| 与知识图谱一致 | 是否与前序结论矛盾 | 🟡 P2 |

### 强制规则
- **必须用 CLI 命令验证**（sed/grep），纯文本阅读不算验证
- **必须独立追踪** ≥1 条调用链，不能只读分析文档打勾
- **对抗分析催眠**: 不能顺着 Researcher 报告线性阅读，必须逆向验证或影子追踪
- **#ifdef 强制交叉验证**：遇到 `#ifdef` / `#if defined()` 包裹的代码块时，必须先查 PROJECT.md 中的目标编译配置确认该宏的状态（活跃/非活跃），不可自行推断宏的真假。review.md 中须注明所依据的 CONFIG 状态
- 宁可升级老板也不放行有"逻辑断层"或"幻觉引用"的报告

### 证据优先原则
当 Reviewer 使用 CLI 命令产出物理证据证明 Researcher 的引用或结论有误时，**Reviewer 的物理证据优先于 Researcher 的语义推论**。Researcher 不得与带有 CLI 验证输出的否定意见争辩，应根据证据修正分析。裁决依据是可验证的物理事实，不是谁的论证更"合理"。

**预处理条件约束**：涉及 `#ifdef` 条件编译区域的代码引用，验证方必须基于 PROJECT.md 中记录的目标编译配置判断该代码路径是否活跃。对死代码路径（当前编译配置下不会编译的代码）的引用不构成有效证据。

## 11. 知识图谱维护

- Jarvis-Arch 在每个模块通过后更新 knowledge-map.md
- Researcher 在开始新模块前必须先读 knowledge-map.md，获取前序模块上下文
- 每条已确认结论必须记录被哪些下游模块引用（用于依赖传导）

### 依赖传导机制

当某核心结论被修正时：
1. Jarvis-Arch 通过 knowledge-map.md 的引用关系，找到所有依赖该结论的下游模块
2. 将这些模块在 tasks-backlog.md 中标记为 [Needs Re-evaluation]
3. [Needs Re-evaluation] 优先级高于新 [TODO]
4. 重审范围：仅限受影响的结论链，不需要全部重做
5. 通知老板级联影响范围

**防循环重审死锁：** 同一模块被重审 >2 次 → 强制 [Escalated]，交由老板仲裁。可选方案：将循环依赖的模块合并为联合分析任务。

### knowledge-map.md 模板

```
# 知识图谱 - [项目名]

## 模块关系图
（mermaid 格式）

## 已确认结论
### 模块 A (analysis/01-module-a.md)
- [C1] 结论描述（引用: file:line）→ 被引用: 模块B, 模块C
- [C2] 结论描述（引用: file:line）→ 被引用: 无

## 需要重审 [Needs Re-evaluation]
- [ ] 模块 B - 原因: 模块 A 结论 C1 已修正（见修正记录 YYYY-MM-DD）

## 开放问题
- [ ] 问题1 - 需要分析模块 D 后才能回答
- [ ] 问题2 - 需要硬件手册

## 修正记录
- [日期] 分析模块 B 时发现模块 A 的 xxx 结论需修正（原因: ...）

## 术语表
- PHY: DDR Physical Layer Interface
- ...
```

## 12. 代码访问策略

代码库可能很大（Linux kernel、U-Boot 等），不 clone 到项目目录或 agent workspace。

**访问方式（按优先级）：**
1. **本地 clone**：如果已 clone，在 PROJECT.md 中记录路径，agent 直接读文件
2. **GitHub API**：通过 `api.github.com` 或 `raw.githubusercontent.com` 按需读文件
3. **web_fetch**：对于其他代码托管平台

**PROJECT.md 必须记录：**
- 仓库 URL
- 分析的 commit hash 或 tag（锁定版本，防止代码变动导致行号失效）
- 本地 clone 路径（如有）
- 访问方式（local / github-api / web）
- 目标编译配置：defconfig 名称 + 关键 CONFIG 选项（决定 #ifdef 分支走向，所有 agent 必须基于同一配置分析）

**路径准确性规则：** 所有 CLI 命令（sed/grep/cat）必须使用相对于代码库根目录的完整路径，禁止依赖当前工作目录（CWD）。大型项目（如 Linux kernel）存在大量同名文件，依赖 CWD 会导致读错文件、引用失效。

## 13. 复盘

每个模块完成后记录复盘三问到 retro.md：
1. **分析效率**: Researcher 追调用链的策略是否高效？
2. **审查发现**: 引用错误率、遗漏率、催眠率
3. **知识图谱修正**: 新模块是否推翻了旧结论？

## 14. 大型项目扩展 (Scaling)

以下机制在中小型项目中不是必须的，但在分析 Linux Kernel、U-Boot 等巨型代码库时至关重要。

### 分级地图 (Tiered Knowledge Map)

当项目极大时，单一 knowledge-map.md 会膨胀导致 Token 爆炸。

**分级方案**：
- `global-map.md`：极精简，只记子系统间的接口、调用关系和共享数据结构
- `.blackboard/subsystem-maps/`：按业务域拆分（如 `usb-map.md`、`memory-map.md`），记录子系统内部细节

**生成规则**：
- **Phase 1 决定**：Jarvis-Arch 侦察后与人类工程师讨论，决定是否分级、如何划分子系统。决定记录在 PROJECT.md
- **初始生成**：Phase 1 由 Jarvis-Arch 生成 global-map.md 初始框架 + subsystem-maps/ 目录结构

**所有权与更新**：
- **只有 Jarvis-Arch 能修改** global-map.md 和 subsystem-maps/*.md
- Phase 2 每个模块完成后，增量更新对应的 subsystem map
- 接口级变化同步到 global-map.md，子系统内部细节不传导

**Researcher 读取规则**：
- Researcher **只读 task.md 里显式指定的 map**，不自行加载其他 map
- Jarvis-Arch 在每个 task.md 中明确列出该任务需要加载的 map 文件

### 上下文剪枝 (Context Pruning)

分析大型代码库时，Researcher 频繁读取源码会快速耗尽 Token 预算。Jarvis-Arch 负责上下文管理：

- **task.md 精准聚焦**：每个 task.md 必须明确限定分析范围和入口点，避免 Researcher 发散
- **knowledge-map.md 只保留核心**：已确认结论只记系统级契约和核心接口，模块内部细节留在 analysis/*.md
- **Researcher 上下文隔离**：每个 Researcher session 只加载当前模块的 task.md + knowledge-map.md + SYMBOL_INDEX.md，不加载其他模块的 analysis/*.md（除非 task.md 显式指定）
- **大文件处理**：Researcher 读源码时应按函数/代码块精确定位（`sed -n 'start,endp'`），禁止整文件读取

## 15. 文件权限

| 文件 | Jarvis-Arch | Researcher | Reviewer |
|------|------------|-----------|---------|
| PROJECT.md | 读写 | 只读 | 只读 |
| tasks-backlog.md | 读写 | 读写（仅改状态） | 只读 |
| task.md | 读写 | 只读 | 只读 |
| analysis/*.md | 只读 | 读写 | 只读 |
| review.md | 只读 | 只读 | 读写 |
| knowledge-map.md | 读写 | 只读 | 只读 |
| SYMBOL_INDEX.md | 读写 | 只读 | 只读 |
| decisions.md | 读写 | 只读 | 只读 |
| retro.md | 读写 | 只读 | 只读 |
| SOUL.md | 只读 | 只读 | 只读 |

## 16. MEMORY.md 与经验传承

**SOUL.md 是只读的** - 定义角色身份，只有老板能修改。任何 agent 禁止修改任何人的 SOUL.md。

**MEMORY.md 是经验外挂** - 每次项目复盘产生的教训，由 Jarvis-Arch 写入对应 agent 目录下的 MEMORY.md。

**启动时必须读 MEMORY.md** - 每个 agent 在开始任何任务前，必须先读取自己目录下的 MEMORY.md 中的历史教训。这是跨项目沉淀"肌肉记忆"的唯一机制。

**经验 ≠ 事实** - MEMORY.md 记录的是历史教训和模式总结，不是代码事实。当 MEMORY.md 中的经验与当前代码的实际观测矛盾时，**以当前代码事实为准**。发现矛盾的 agent 应报告给 Jarvis-Arch，由 **Jarvis-Arch 统一更新 MEMORY.md**（保持单一写原则）。

如果某条经验足够重要需要写入 SOUL.md，向老板提议，由老板审批后修改。

## 17. 禁止事项（全员）

- 禁止修改任何人的 SOUL.md（只有老板能改）
- 禁止修改 PROTOCOL.md
- 禁止无代码引用的分析结论
- 禁止并行分析（同一时刻最多一个 [Analyzing]）
- 禁止跳过 guard.sh 检查
- 禁止跳过 sign-off 门禁
