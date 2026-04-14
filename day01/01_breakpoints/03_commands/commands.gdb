# GDB script to demonstrate enabling the SECOND breakpoint from the FIRST one

# 1. Set the trigger breakpoint (Breakpoint 1)
break PrepareWork

# 2. Set the main logging breakpoint (Breakpoint 2)
break ProcessData
# Disable it immediately so it doesn't trigger until Breakpoint 1 is hit
disable 2

# 3. Actions for the trigger breakpoint (Breakpoint 1)
commands 1
    silent
    printf ">>> GDB: PrepareWork (BP 1) hit. Enabling intensive logging (BP 2).\n"
    enable 2
    continue
end

# 4. Actions for the logging breakpoint (Breakpoint 2)
commands 2
    silent
    printf "GDB SPY: ID=%d, Info=\"%s\"\n", id, info.c_str()
    continue
end

# 5. Run the program
run
