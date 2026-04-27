# Demo: Attaching GDB to a Running Process

This demo simulates a "Secure Vault System" running as a background service. It demonstrates how to attach GDB to a process that is already running and inspect its internal state.

## Objectives
- Learn how to find a Process ID (PID).
- Attach GDB to a running process using `-p` or the `attach` command.
- Practice basic navigation and variable inspection.

## 1. Build and Run
First, build and start the "Secure Vault":

```bash
# Build the project
bash build.sh --full-build

# Run the vault simulation
./build/attachable_exe
```

The program will display its **Process ID (PID)** and start printing heartbeats.

## 2. The GDB Challenge
Open a **new terminal** and follow these steps to "breach" the vault.

### Step A: Attach GDB
Use the PID displayed by the program (replace `<PID>` below):
```bash
gdb -p <PID>
```
*Note: GDB will immediately pause the program upon attachment.*

### Step B: Inspect the System
Explore the running state:
```gdb

# Inspect the "globalVault" state
(gdb) p globalVault
```

### Step C: Hack the Vault
The system is currently secure because `m_isUnlocked` is false. You can use GDB to manipulate the program's memory and "hack" the vault:

```gdb
# Change the value of the flag directly in memory
(gdb) set globalVault.m_isUnlocked = 1

# Verify the change
(gdb) p globalVault.m_isUnlocked
```

### Step D: Resume and Observe
Resume the program to see the effect of your "hack":
```gdb
(gdb) continue
```
Switch back to the **first terminal**. The system status should now show `!!! COMPROMISED !!!` and reveal the secret payload.

---
## Key GDB Commands Used
| Command | Description |
|---------|-------------|
| `gdb -p <PID>` | Start GDB and attach to the specified process |
| `print <expr>` | (or `p`) Evaluate and print an expression |
| `set <var>=<val>`| Change the value of a variable in memory |
| `detach`       | Stop debugging the process but leave it running |
