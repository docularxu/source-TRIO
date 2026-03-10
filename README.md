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

### 2. Verify Agent Communication

Before starting real work, run a quick smoke test to confirm all three agents can talk to each other:

```
You → Jarvis-Arch:  "Ping Researcher and Reviewer, confirm communication."
```

Expected result:
- Jarvis-Arch sends a message to Researcher → Researcher acknowledges
- Jarvis-Arch sends a message to Reviewer → Reviewer acknowledges
- Jarvis-Arch reports back: "All agents online, communication verified."

If any link fails, check your engine's agent-to-agent communication config (see `configs/openclaw-example.yaml`).

### 3. Start Analyzing

Point Jarvis-Arch to a source code project (an existing local clone or a GitHub repo URL):

> "Analyze the dummy-c-project at example_workspace/dummy-c-project/. Focus on tracing the init flow in main() to find out which hardware register address is actually being operated on."

Jarvis-Arch will handle the rest (Phase 1 - Reconnaissance):
1. Align the analysis scope with you through conversation
2. Initialize the workspace using templates (`PROJECT.md`, `.blackboard/`, etc.)
3. Scout the codebase, produce `PROJECT.md` + `SYMBOL_INDEX.md`
4. Submit the plan to Reviewer, then ask you for sign-off

After sign-off, Phase 2 begins automatically:
5. Dispatch modules to Researcher, trigger Reviewer for verification
6. Integrate results into `knowledge-map.md`

> **Tip**: Templates in `templates/` are reference copies. You can also manually initialize a workspace with `cp -r templates/blackboard/ .blackboard/` if you prefer hands-on setup.

### 4. See It in Action

`example_workspace/dummy-c-project/` contains a 3-file C project with deliberate traps (function pointer dispatch, macro obfuscation, dead code). Point your agents at it and run the full workflow.

After your agents finish, compare your results against `example_output/dummy-c-project/` - the reference output from a real live-fire exercise.

## 📁 Repository Structure

```
source-driven-agent-framework/
├── README.md
├── LICENSE
├── agents/                         # Agent System Prompts (SOUL.md)
│   ├── jarvis-arch/SOUL.md         #   Architect / Coordinator
│   ├── researcher/SOUL.md          #   Code Archaeologist
│   └── reviewer/SOUL.md            #   Logic Judge
├── configs/
│   └── openclaw-example.yaml       # OpenClaw engine configuration example
├── templates/                      # Blank templates - copy to start a new project
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
│   └── design-architecture.html    # Full design document (interactive)
├── example_workspace/              # Test target - clean source code, no output
│   └── dummy-c-project/
│       ├── README.md               # Trap descriptions and expected results
│       ├── main.c                  # Entry point with implicit dispatch
│       ├── driver_api.h            # Interface contract + dead code decoy
│       └── usb_driver.c            # Real implementation + macro obfuscation
└── example_output/                 # Reference output - compare after your run
    └── dummy-c-project/
        └── .blackboard/
            ├── PROJECT.md          # Project overview (generated by Jarvis-Arch)
            ├── SYMBOL_INDEX.md     # Global symbol table
            ├── tasks-backlog.md    # Task list with final states
            ├── knowledge-map.md    # Knowledge graph
            ├── REPORT.md           # Final integrated report
            └── analysis/
                ├── task-01-init-flow.md     # Analysis task definition
                ├── report-01-init-flow.md   # Researcher's analysis report
                └── review-01-init-flow.md   # Reviewer's verification record
```

## 💡 Experience Accumulation

"Permanent team, rotating projects." When a project wraps up, don't delete the agents. Append lessons learned to each agent's `MEMORY.md` (SOUL.md is read-only - only the human owner modifies it). Your team gets sharper with every project.

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
