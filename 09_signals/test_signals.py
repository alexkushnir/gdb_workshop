import subprocess
import signal
import sys
import os

def test_signal(input_str, expected_signal, fatal=False):
    print(f"Testing input: {input_str.strip()} (Expecting signal {expected_signal})")
    
    # We use stdbuf to disable buffering if possible, but python's subprocess interacting with C++ std::print might be buffered.
    # However, we mostly care about exit code and output content.
    
    proc = subprocess.Popen(
        ['./display_and_call'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        cwd='./build',
        text=True
    )
    
    stdout, stderr = proc.communicate(input=input_str)
    
    print(f"Output:\n{stdout}")
    if stderr:
        print(f"Stderr:\n{stderr}")
        
    if fatal:
        # Fatal signals cause exit with code = signum (in my handler implementation)
        # Verify exit code
        if proc.returncode == expected_signal:
            print("PASS: Exit code matches signal number.")
        else:
            print(f"FAIL: Exit code {proc.returncode} != {expected_signal}")
            return False
            
        if f"Caught signal {expected_signal}" in stdout:
            print("PASS: Output contains caught signal message.")
        else:
            print("FAIL: Output missing caught signal message.")
            return False
    else:
        # Non-fatal signals should allow continuing.
        # We append '0' to exit cleanly.
        if proc.returncode == 0:
            print("PASS: Exited cleanly.")
        else:
            print(f"FAIL: Exit code {proc.returncode} != 0")
            return False
            
        if f"Caught signal {expected_signal}" in stdout:
            print("PASS: Output contains caught signal message.")
        else:
            print("FAIL: Output missing caught signal message.")
            return False
            
    return True

failed = False

# 1. SIGSEGV (11)
if not test_signal("1\n", 11, fatal=True): failed = True

# 2. SIGFPE (8)
if not test_signal("2\n", 8, fatal=True): failed = True

# 3. SIGINT (2)
if not test_signal("3\n0\n", 2, fatal=False): failed = True

# 4. SIGTERM (15)
if not test_signal("4\n0\n", 15, fatal=False): failed = True

# 5. SIGABRT (6)
if not test_signal("5\n", 6, fatal=True): failed = True

# 6. SIGUSR1 (10 or 16 depending on arch, usually 10 on x86/ARM Linux)
# We can't be sure of the number, but we can check output.
# Actually, I used signal.SIGUSR1 in python to check?
# Let's just check if it finds "Caught signal" and exits cleanly.
print("Testing input: 6\n0\n (Expecting SIGUSR1)")
proc = subprocess.Popen(
    ['./display_and_call'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    cwd='./build',
    text=True
)
stdout, stderr = proc.communicate(input="6\n0\n")
print(f"Output:\n{stdout}")
if "Caught signal" in stdout and "User defined signal 1" in stdout:
    print("PASS: Output contains caught signal message.")
else:
    print("FAIL: Output missing caught signal message.")
    failed = True

if failed:
    sys.exit(1)
else:
    print("ALL TESTS PASSED")
    sys.exit(0)
