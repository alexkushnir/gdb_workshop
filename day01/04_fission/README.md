# GDB Fission (Split DWARF) Demonstration

This project demonstrates the **Fission** feature in GDB, which significantly reduces the size of executables by splitting debug information into separate files.

## What is Fission?
Standard debug info (`-g`) embeds all DWARF data (types, line numbers, local variables) directly into the executable. 
**Fission** (`-gsplit-dwarf`) moves the bulk of this data into external `.dwo` files, leaving only a small **skeleton** in the binary.

## How to Build
Use the provided `build.sh` script:
```bash
# Full build with Fission enabled
bash build.sh --full-build --split-dwarf
```

## Key Lessons

### 1. The Skeleton is Essential
Even though most debug info is external, the executable **must** keep its skeleton debug info and symbol table. 
- **DO NOT** use `-s` or the `strip` command. 
- Stripping the binary removes the skeleton, leaving GDB with no way to find the `.dwo` files.

### 2. How GDB Finds `.dwo` Files
GDB uses absolute paths recorded in the binary's skeleton (`DW_AT_comp_dir`) to find the `.dwo` files. 
- If you move the executable but keep the `build` directory, GDB will still find the debug info.
- If you delete or rename the `build` directory, GDB will fail to load symbols for those files.

## Demonstration Steps
1. **Build with Fission**: `bash build.sh --full-build --split-dwarf`
2. **Verify it works**: `gdb ./fission_demo` (you should see symbols and source).
3. **Hide the debug info**: `mv build build_hidden`
4. **Try to debug again**: `gdb ./fission_demo`
   - You will see errors like: `During symbol reading: Could not find DWO CU...`
   - GDB can no longer see local variables or line numbers.
5. **Restore**: `mv build_hidden build`
