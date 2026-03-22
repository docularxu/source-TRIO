#!/bin/bash
# guard.sh - State-change enforcement for source-TRIO
# Called before modifying tasks-backlog.md status tags.
# Returns non-zero exit code to block violations.

# TODO: Implement guards
# - Serial enforcement: block [Analyzing] if another task is already [Analyzing]
# - File existence checks before state transitions

echo "OK"
exit 0
