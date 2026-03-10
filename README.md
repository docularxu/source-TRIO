# 🔬 Source-Driven Code Analysis Multi-Agent Framework

A multi-agent framework designed for deep code reverse engineering and architecture analysis.

In software development, we use Spec-Driven agents to write code. But in reverse engineering and source code analysis, we need **Source-Driven** agents - where the source code itself is the single source of truth.

This framework introduces **"No Evidence, No Claim"** and **anti-Analysis Hypnosis** mechanisms to tackle million-line-scale C/C++ system code analysis.

## 🌟 Core Design Principles

1. **No Evidence, No Claim**: Every business logic assertion must be accompanied by a precise physical code reference `[file:line]`. No reference, no claim.

2. **Zero-Trust Review**: The Reviewer agent is equipped with CLI tool permissions (`sed`, `grep`) to physically verify code citations. Pure text "reading" does not count as verification.

3. **Progressive Knowledge Graph**: Instead of single-pass large-context reads, the framework uses a shared blackboard (`tasks-backlog` + `knowledge-map`) to progressively piece together the global architecture, module by module.

4. **Blackboard State Machine**: All workflow progression is driven by file-based state tags (`[TODO]` → `[Analyzing]` → `[Reviewing]` → `[Done]`), not by agent memory. This enables crash recovery and deterministic flow control.

## 👥 The Agent Team

A "permanent team, rotating projects" approach - the agents persist across projects, accumulating expertise over time.

- **⚡ Jarvis-Arch (Architect)**: Scouts the codebase, splits modules, controls token budget, manages the knowledge graph, and arbitrates conflicts. Drives the state machine.

- **🔬 Researcher (Code Archaeologist)**: Traces call chains, tracks data flow mutations, reports implicit jumps (`[UNRESOLVED]`). Believes only in what the compiler executes, not what comments say.

- **🔍 Reviewer (Logic Judge)**: Assumes all reports contain hallucinations until physically proven otherwise. Uses reverse verification, shadow tracking, and context toxicity checks to catch errors.

## 🚀 Quick Start

### 1. Deploy the Prompts

Clone this repo. Copy `agents/` directory and use each `SOUL.md` as the System Prompt for your multi-agent engine (OpenClaw, AutoGen, LangGraph, etc.).

See `configs/openclaw-example.yaml` for an OpenClaw configuration example with tool permissions.

### 2. Initialize a Project Workspace

```bash
mkdir my-analysis && cd my-analysis
cp -r /path/to/this/repo/templates/blackboard/ .blackboard/
cp /path/to/this/repo/templates/PROJECT.md ./PROJECT.md
# Edit PROJECT.md: fill in repo URL, commit hash, module plan
```

### 3. Start Analyzing

Tell Jarvis-Arch your analysis target:

> "Analyze the DDR initialization subsystem in the K1 U-Boot codebase. Start with a reconnaissance of the directory structure and produce a module split plan."

Jarvis-Arch will:
1. Align the analysis scope with you
2. Scout the codebase, produce `PROJECT.md` + `SYMBOL_INDEX.md`
3. Dispatch modules to Researcher, trigger Reviewer for verification
4. Integrate results into `knowledge-map.md`

### 4. See It in Action

Check `example_workspace/` for a completed micro-demo - a 3-file C project with function pointers, `#ifdef` branches, and `__weak` overrides, fully analyzed by the agent team.

## 📁 Repository Structure

```
source-driven-agent-framework/
├── README.md
├── agents/                     # Agent System Prompts (SOUL.md)
│   ├── jarvis-arch/SOUL.md
│   ├── researcher/SOUL.md
│   └── reviewer/SOUL.md
├── configs/
│   └── openclaw-example.yaml   # Engine configuration with tool permissions
├── templates/                  # Copy these to start a new project
│   ├── PROJECT.md
│   ├── blackboard/
│   │   ├── tasks-backlog.md
│   │   ├── task.md
│   │   ├── knowledge-map.md
│   │   ├── review.md
│   │   ├── decisions.md
│   │   └── retro.md
│   └── analysis/
│       └── NN-module-name.md
├── docs/
│   └── design-architecture.html  # Full design document (interactive)
└── example_workspace/          # Completed micro-demo
    ├── dummy-c-project/        # 3 C files with implicit jumps
    ├── .blackboard/            # Final blackboard state
    └── analysis/               # Generated analysis reports
```

## 💡 Experience Accumulation

"Permanent team, rotating projects." When a project wraps up, don't delete the agents. Append lessons learned to each agent's `== Learned Rules ==` section in their SOUL.md. Your team gets sharper with every project.

## Three-Phase Workflow

| Phase | Input | Output | Gate |
|-------|-------|--------|------|
| **1. Reconnaissance** | Analysis target + codebase access | PROJECT.md, tasks-backlog.md, knowledge-map.md (initial), SYMBOL_INDEX.md | Reviewer pass + Owner sign-off |
| **2. Deep Dive** (per module) | task.md + knowledge-map + source code | analysis/NN-module.md, review.md, knowledge-map (updated) | Reviewer pass (≤3 rounds) |
| **3. Final Report** | All analysis/*.md + knowledge-map (final) | REPORT.md | Owner sign-off |

## License

MIT

---

*Designed by Guodong Xu - [docularxu](https://github.com/docularxu)*
