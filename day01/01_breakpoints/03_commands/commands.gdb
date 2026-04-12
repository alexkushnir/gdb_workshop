# GDB script to demonstrate breakpoint commands

# 1. Set a breakpoint on the target function
break process_data

# 2. Define the actions to take when the breakpoint is hit
commands
    silent
    printf "GDB SPY: ID=%d, Info=\"%s\"\n", id, info.c_str()
    continue
end

# 3. Run the program
run
