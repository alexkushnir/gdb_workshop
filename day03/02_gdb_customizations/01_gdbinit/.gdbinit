# .gdbinit for the gdbinit demo

add-auto-load-safe-path /home/alex/gdb_workshop

# 1. Basic Settings
set print pretty on
set print object on
set print static-members on
set print vtbl on
set confirm off
set pagination off

# 2. History Settings
set history save on
set history size 1000
set history filename .gdb_history

# 3. Custom Commands (Macros)
define print_vec
    printf "Vector size: %lu\n", $arg0.size()
    set $i = 0
    while $i < $arg0.size()
        print $arg0[$i]
        set $i = $i + 1
    end
end
document print_vec
    Custom command to print elements of a std::vector.
    Usage: print_vec <vector_variable>
end

define status
    printf "--- GDB Status ---\n"
    info breakpoints
    info threads
    printf "------------------\n"
end
document status
    Custom command to show current GDB status (breakpoints and threads).
end

# 4. Automated Breakpoints
break main

# 5. Startup message
printf "\n"
printf "***********************************************\n"
printf "*   GDB Workshop: .gdbinit Demo Loaded!       *\n"
printf "***********************************************\n"
printf "* Custom commands available:                  *\n"
printf "*   - print_vec <vector>                      *\n"
printf "*   - status                                  *\n"
printf "***********************************************\n"
printf "\n"
