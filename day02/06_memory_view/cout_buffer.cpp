// cout_buffer.cpp — where std::cout's unflushed bytes actually live.
//
//   g++ -std=c++20 -g -O0 -fno-omit-frame-pointer cout_buffer.cpp -o cout_buffer
//   gdb -q ./cout_buffer
//   (gdb) b 21                               <- the "return 0" line
//   (gdb) run
//   (gdb) p *stdout
//   (gdb) x/s stdout->_IO_write_base
//

#include <iostream>

int main() {
    std::cout << "Hello";
	std::cout << " Goodbye "; 
	std::cout << " OK " << std::endl;
	std::cout << "New chars......";
	// break here (b 21), then: p *stdout   /  x/s stdout->_IO_write_base
	// (glibc: stdout == &_IO_2_1_stdout_)
	
	return 0;
}
