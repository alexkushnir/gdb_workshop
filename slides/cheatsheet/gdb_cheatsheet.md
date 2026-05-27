<div style="text-align: center; page-break-after: always; padding-top: 60px;">
  <img src="Claritune-Logo.png" alt="Claritune Logo" style="max-width: 300px; margin-bottom: 40px;">
  
  <h1 style="font-size: 2.5em; margin: 40px 0;">GDB Cheatsheet</h1>
  
  <div style="margin-top: 80px; font-size: 1.2em;">
    <p style="margin: 10px 0;">By</p>
    <p style="margin: 10px 0; font-weight: bold;">Alex Kushnir</p>
    <p style="margin: 10px 0; font-weight: bold;">Amir Kirsh</p>
  </div>
</div>

<div class="section-block">
  <h2>Starting GDB</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>gdb &lt;program&gt;</code></td>
      <td>Start GDB.</td>
    </tr>
    <tr>
      <td><code>gdb &lt;program&gt; [-c &lt;core dump&gt;]</code></td>
      <td>Start GDB, optionally with a core dump.</td>
    </tr>
    <tr>
      <td><code>gdb --args &lt;program&gt; &lt;args...&gt;</code></td>
      <td>Start GDB and pass program arguments.</td>
    </tr>
    <tr>
      <td><code>gdb --pid &lt;pid&gt;</code></td>
      <td>Attach to an already running process.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Basic commands</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>set args &lt;args...&gt;</code></td>
      <td>Set arguments for later <code>run</code> commands.</td>
    </tr>
    <tr>
      <td><code>show args</code></td>
      <td>Show currently configured arguments.</td>
    </tr>
    <tr>
      <td><code>run</code></td>
      <td>Run the program.</td>
    </tr>
    <tr>
      <td><code>run &lt; input.txt</code></td>
      <td>Run with stdin redirected from a file.</td>
    </tr>
    <tr>
      <td><code>run 2&gt;errors.log</code></td>
      <td>Run with stderr redirected to a file.</td>
    </tr>
    <tr>
      <td><code>start</code></td>
      <td>Run and stop at <code>main</code>.</td>
    </tr>
    <tr>
      <td><code>kill</code></td>
      <td>Kill the running program.</td>
    </tr>
    <tr>
      <td><code>help</code></td>
      <td>Show help on GDB commands and topics.</td>
    </tr>
    <tr>
      <td><code>quit</code></td>
      <td>Exit GDB.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>TUI commands</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>gdb --tui &lt;program&gt;</code> / <code>tui enable</code></td>
      <td>Enable the TUI mode.</td>
    </tr>
    <tr>
      <td><code>layout src</code>, <code>layout asm</code>, <code>layout regs</code></td>
      <td>Switch visible TUI layouts.</td>
    </tr>
    <tr>
      <td><code>layout prev</code> / <code>layout next</code></td>
      <td>Cycle layouts.</td>
    </tr>
    <tr>
      <td><code>tui focus &lt;layout&gt;</code></td>
      <td>Move focus to a specific TUI pane.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Stack and stepping</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>backtrace</code> / <code>where</code></td>
      <td>Show the call stack.</td>
    </tr>
    <tr>
      <td><code>backtrace full</code> / <code>where full</code></td>
      <td>Show the call stack with locals.</td>
    </tr>
    <tr>
      <td><code>frame &lt;frame#&gt;</code></td>
      <td>Select a stack frame.</td>
    </tr>
    <tr>
      <td><code>up</code> / <code>down</code></td>
      <td>Move between stack frames.</td>
    </tr>
    <tr>
      <td><code>step</code></td>
      <td>Step into the next source line or call.</td>
    </tr>
    <tr>
      <td><code>stepi</code></td>
      <td>Step into the next instruction (Assembly).</td>
    </tr>
    <tr>
      <td><code>next</code></td>
      <td>Step over the next source line or call.</td>
    </tr>
    <tr>
      <td><code>nexti</code></td>
      <td>Step over the next instruction (Assembly).</td>
    </tr>
    <tr>
      <td><code>finish</code></td>
      <td>Continue until the current function returns.</td>
    </tr>
    <tr>
      <td><code>continue</code></td>
      <td>Resume normal execution.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Breakpoints</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>break &lt;where&gt;</code></td>
      <td>Set a new breakpoint.</td>
    </tr>
    <tr>
      <td><code>break file.cpp:42</code></td>
      <td>Break at the given source line.</td>
    </tr>
    <tr>
      <td><code>break function_name</code></td>
      <td>Break at function entry.</td>
    </tr>
    <tr>
      <td><code>break Class::Method</code></td>
      <td>Break at class member function entry.</td>
    </tr>
    <tr>
      <td><code>break *0x00001234</code></td>
      <td>Break at a code address.</td>
    </tr>
    <tr>
      <td><code>delete &lt;breakpoint#&gt;</code></td>
      <td>Remove a breakpoint.</td>
    </tr>
    <tr>
      <td><code>clear</code></td>
      <td>Delete all breakpoints.</td>
    </tr>
    <tr>
      <td><code>clear file.cpp:42</code></td>
      <td>Delete breakpoints at a specific line.</td>
    </tr>
    <tr>
      <td><code>enable &lt;breakpoint#&gt;</code> / <code>disable &lt;breakpoint#&gt;</code></td>
      <td>Enable or disable an existing breakpoint.</td>
    </tr>
    <tr>
      <td><code>info breakpoints</code></td>
      <td>List breakpoints, watchpoints, and IDs.</td>
    </tr>
    <tr>
      <td><code>set breakpoint pending on</code></td>
      <td>Allow breakpoints in not-yet-loaded code.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Conditional breakpoints and actions</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>break &lt;where&gt; if &lt;condition&gt;</code></td>
      <td>Create a conditional breakpoint or watchpoint.</td>
    </tr>
    <tr>
      <td><code>condition &lt;breakpoint#&gt; &lt;condition&gt;</code></td>
      <td>Set or update the condition of an existing breakpoint.</td>
    </tr>
    <tr>
      <td><code>ignore &lt;id&gt; 5</code></td>
      <td>Ignore the next N hits of a breakpoint.</td>
    </tr>
    <tr>
      <td><code>commands &lt;id&gt; ... end</code></td>
      <td>Run commands when a breakpoint is hit.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h3>Example: commands usage</h3>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>break foo if x == 0</code></td>
      <td>Create a conditional breakpoint that only stops when <code>x</code> equals zero.</td>
    </tr>
    <tr>
      <td><code>commands 1</code><br><code>printf "x is zero\n"</code><br><code>continue</code><br><code>end</code></td>
      <td>Run a small command list when breakpoint 1 is hit, then continue execution automatically.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Variables and memory</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>print/format &lt;what&gt;</code></td>
      <td>Print the content of a variable, memory location, or register.</td>
    </tr>
    <tr>
      <td><code>print variable</code></td>
      <td>Print a value with default formatting.</td>
    </tr>
    <tr>
      <td><code>print *ptr</code></td>
      <td>Dereference a pointer.</td>
    </tr>
    <tr>
      <td><code>print *arr@N</code></td>
      <td>Print N array entries starting at <code>arr</code>.</td>
    </tr>
    <tr>
      <td><code>display/format &lt;what&gt;</code></td>
      <td>Print automatically after each step or stop.</td>
    </tr>
    <tr>
      <td><code>display variable</code></td>
      <td>Auto-print a variable.</td>
    </tr>
    <tr>
      <td><code>undisplay &lt;display#&gt;</code></td>
      <td>Remove a display expression.</td>
    </tr>
    <tr>
      <td><code>enable display &lt;display#&gt;</code> / <code>disable display &lt;display#&gt;</code></td>
      <td>Enable or disable a display expression.</td>
    </tr>
    <tr>
      <td><code>info proc mappings</code></td>
      <td>Show process memory layout and mapped regions.</td>
    </tr>
    <tr>
      <td><code>x/nfu &lt;address&gt;</code></td>
      <td>Examine memory at an address.</td>
    </tr>
    <tr>
      <td><code>x/[COUNT][SIZE][FORMAT] &lt;address&gt;</code></td>
      <td>General memory examine form.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h3>x/ formatting</h3>
  <table>
  <thead>
    <tr>
      <th>Option</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Count</td>
      <td>number of units to print.</td>
    </tr>
    <tr>
      <td>Size</td>
      <td><code>b</code> byte, <code>h</code> half-word, <code>w</code> word, <code>g</code> giant word.</td>
    </tr>
    <tr>
      <td>Format</td>
      <td><code>a</code> pointer, <code>c</code> char, <code>d</code> signed decimal, <code>f</code> float, <code>o</code> octal, <code>s</code> string, <code>t</code> binary, <code>u</code> unsigned decimal, <code>x</code> hexadecimal, <code>i</code> instruction.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h3>x/ formatting example</h3>
  <table>
  <thead>
    <tr>
      <th>Example Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>x/4xw &amp;my_var</code></td>
      <td>Examine 4 words (<code>w</code>) of memory starting at <code>&amp;my_var</code> formatted as hexadecimal (<code>x</code>).</td>
    </tr>
    <tr>
      <td><code>x/10i $eip</code></td>
      <td>Examine (disassemble) the next 10 instructions (<code>i</code>) starting from the instruction pointer register (<code>$eip</code>).</td>
    </tr>
    <tr>
      <td><code>x/8cb str_ptr</code></td>
      <td>Examine 8 characters (<code>c</code>), 1 byte each (<code>b</code>), starting at address <code>str_ptr</code>.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Watchpoints</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>watch &lt;where&gt;</code></td>
      <td>Set a new watchpoint.</td>
    </tr>
    <tr>
      <td><code>watch variable_name</code></td>
      <td>Break on write.</td>
    </tr>
    <tr>
      <td><code>rwatch variable_name</code></td>
      <td>Break on read.</td>
    </tr>
    <tr>
      <td><code>awatch variable_name</code></td>
      <td>Break on read or write.</td>
    </tr>
    <tr>
      <td><code>watch &lt;where&gt;</code></td>
      <td>Set a new watchpoint.</td>
    </tr>
    <tr>
      <td><code>watch variable_name</code></td>
      <td>Break on write.</td>
    </tr>
    <tr>
      <td><code>rwatch variable_name</code></td>
      <td>Break on read.</td>
    </tr>
    <tr>
      <td><code>awatch variable_name</code></td>
      <td>Break on read or write.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Signals handling</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>handle &lt;signal&gt; &lt;options&gt;</code></td>
      <td>Control whether a signal is printed, stops execution, or is passed to the program.</td>
    </tr>
    <tr>
      <td><code>handle SIGSEGV stop print</code></td>
      <td>Stop and print when SIGSEGV occurs.</td>
    </tr>
    <tr>
      <td><code>handle SIGPIPE nostop noprint pass</code></td>
      <td>Let the program handle SIGPIPE.</td>
    </tr>
    <tr>
      <td><code>handle SIGUSR1 nopass</code></td>
      <td>Catch SIGUSR1 without passing it through.</td>
    </tr>
    <tr>
      <td><code>signal SIGUSR1</code></td>
      <td>Send a signal to the inferior.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Altering execution</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>set var &lt;variable_name&gt;=&lt;value&gt;</code> / <code>set variable x = 10</code></td>
      <td>Change a variable at runtime.</td>
    </tr>
    <tr>
      <td><code>set $rax = 0</code></td>
      <td>Change a register directly.</td>
    </tr>
    <tr>
      <td><code>return &lt;expression&gt;</code></td>
      <td>Force the current function to return immediately.</td>
    </tr>
    <tr>
      <td><code>jump file.cpp:100</code></td>
      <td>Move execution to a different source line.</td>
    </tr>
    <tr>
      <td><code>call function(args)</code></td>
      <td>Invoke a function manually from GDB.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Information commands</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>info args</code></td>
      <td>Show arguments in the current frame.</td>
    </tr>
    <tr>
      <td><code>info locals</code></td>
      <td>Show locals in the current frame.</td>
    </tr>
    <tr>
      <td><code>info display</code></td>
      <td>List display expressions.</td>
    </tr>
    <tr>
      <td><code>info sharedlibrary</code></td>
      <td>List loaded shared libraries.</td>
    </tr>
    <tr>
      <td><code>info signals</code></td>
      <td>List signals and their handling policy.</td>
    </tr>
    <tr>
      <td><code>info threads</code></td>
      <td>List all threads.</td>
    </tr>
    <tr>
      <td><code>whatis variable_name</code></td>
      <td>Show variable type.</td>
    </tr>
    <tr>
      <td><code>ptype MyClass</code></td>
      <td>Show class or structure layout.</td>
    </tr>
    <tr>
      <td><code>disassemble</code> / <code>disassemble &lt;where&gt;</code></td>
      <td>Disassemble the current function or location.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Source code manipulation</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>directory &lt;directory&gt;</code></td>
      <td>Add a source search directory.</td>
    </tr>
    <tr>
      <td><code>show directories</code></td>
      <td>List configured source directories.</td>
    </tr>
    <tr>
      <td><code>list</code></td>
      <td>Show current source context.</td>
    </tr>
    <tr>
      <td><code>list &lt;filename&gt;:&lt;function&gt;</code></td>
      <td>Show source around a function.</td>
    </tr>
    <tr>
      <td><code>list &lt;filename&gt;:&lt;line_number&gt;</code></td>
      <td>Show source around a line.</td>
    </tr>
    <tr>
      <td><code>set listsize &lt;count&gt;</code> / <code>show listsize</code></td>
      <td>Set or show number of source lines displayed by <code>list</code>.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Debugging multithreaded programs</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>thread &lt;thread#&gt;</code></td>
      <td>Choose the thread to operate on.</td>
    </tr>
    <tr>
      <td><code>thread apply all &lt;command&gt;</code></td>
      <td>Run a command on every thread.</td>
    </tr>
    <tr>
      <td><code>break file.cpp:42 thread 3</code></td>
      <td>Create a breakpoint only for one thread.</td>
    </tr>
    <tr>
      <td><code>set scheduler-lock off|on|step</code></td>
      <td>In all-stop mode only: adjust during the session to control whether other threads run while stepping (<code>off</code>=all run, <code>on</code>=only current, <code>step</code>=locked while stepping only).</td>
    </tr>
    <tr>
      <td><code>set non-stop on|off</code></td>
      <td>Choose the thread stop mode at the beginning of the debug session. Use <code>off</code> for all-stop (default) or <code>on</code> for non-stop mode, where threads stop independently.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Tracepoints</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>trace function_name</code> / <code>trace file.c:42</code></td>
      <td>Create a tracepoint.</td>
    </tr>
    <tr>
      <td><code>actions</code> ... <code>collect $regs,$locals,myvar</code> ... <code>end</code></td>
      <td>Specify what to capture at a tracepoint.</td>
    </tr>
    <tr>
      <td><code>passcount 100 1</code></td>
      <td>Collect the first 100 hits, then stop.</td>
    </tr>
    <tr>
      <td><code>tstart</code>, <code>tstop</code></td>
      <td>Start or stop tracepoint collection.</td>
    </tr>
    <tr>
      <td><code>tfind</code>, <code>tdump</code>, <code>tstatus</code></td>
      <td>Inspect tracepoint results and collection status.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h3>Example: actions usage</h3>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>trace foo</code></td>
      <td>Create a tracepoint at function <code>foo</code>.</td>
    </tr>
    <tr>
      <td><code>actions</code><br><code>collect $pc,$eax</code><br><code>collect my_var</code><br><code>end</code></td>
      <td>Capture registers and a variable each time the tracepoint hits.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>Reverse execution (time travelling)</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>record full</code></td>
      <td>Enable reverse debugging.</td>
    </tr>
    <tr>
      <td><code>reverse-step</code>, <code>reverse-next</code>, <code>reverse-continue</code></td>
      <td>Step or continue backward.</td>
    </tr>
  </tbody>
</table>
</div>

<div class="section-block">
  <h2>GDB and Valgrind integration</h2>
  <table>
  <thead>
    <tr>
      <th>Command</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>valgrind --vgdb=yes --vgdb-error=0 --vgdb-stop-at=startup &lt;app&gt;</code></td>
      <td>Launch under Valgrind and stop in a GDB-connectable state.</td>
    </tr>
    <tr>
      <td><code>target remote | vgdb</code></td>
      <td>Connect GDB to Valgrind.</td>
    </tr>
    <tr>
      <td><code>monitor leak_check</code> / <code>monitor v.info location &lt;addr&gt;</code></td>
      <td>Query Valgrind while debugging.</td>
    </tr>
  </tbody>
</table>
</div>
