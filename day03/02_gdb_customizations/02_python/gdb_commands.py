import gdb

class SummarizeFrame(gdb.Command):
    """Summarize all local variables in the current stack frame.
    
    This command iterates through symbols in the current frame and prints
    their names, types, and values in a formatted table.
    """

    def __init__(self):
        super(SummarizeFrame, self).__init__("summarize-frame", gdb.COMMAND_DATA)

    def invoke(self, arg, from_tty):
        # Obtain the current selected frame
        try:
            frame = gdb.selected_frame()
        except gdb.error as e:
            print(f"Error: {e}")
            return

        # Obtain the block associated with the frame's PC
        block = frame.block()
        
        print("\n" + "="*60)
        print(f"{'Variable':<20} | {'Type':<20} | {'Value'}")
        print("-" * 60)

        # Iterate through symbols in the block
        seen_vars = set()
        while block:
            for symbol in block:
                if symbol.is_argument or symbol.is_variable:
                    name = symbol.name
                    if name in seen_vars:
                        continue
                    seen_vars.add(name)
                    
                    try:
                        # Fetch the value of the symbol in the context of the frame
                        value = symbol.value(frame)
                        type_str = str(symbol.type)
                        
                        # Format output
                        print(f"{name:<20} | {type_str:<20} | {value}")
                    except Exception as e:
                        print(f"{name:<20} | Error fetching value: {e}")
            
            # Walk up the block scope (nested blocks, function block, etc.)
            block = block.superblock

        print("="*60 + "\n")

# Instantiate the command to register it with GDB
SummarizeFrame()

print("Python script loaded: 'summarize-frame' command is now available.")
