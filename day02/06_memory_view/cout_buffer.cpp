#include <iostream>

int main() {
    std::cout << "Hello";
	std::cout << " Goodbye "; 
	std::cout << " OK " << std::endl;
	std::cout << "New chars......";
	// break here (b 10), then: p *stdout   /  x/s stdout->_IO_write_base
	// (glibc: stdout == &_IO_2_1_stdout_)
	
	return 0;
}
