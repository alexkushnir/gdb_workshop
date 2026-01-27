#include <print>
#include <vector>

#define PRINT_HELLO() auto count = 0; while (count < 10) { PrintHello(); ++count; }

void PrintHello() 
{
    std::println("Hello, World!");
}

void InnerFunction(std::vector<int>& vec, int idx) 
{
    vec[idx] = 42;  // Potential out-of-bounds crash
}

void OuterFunction(int size) 
{
    std::vector<int> vec(size);
    InnerFunction(vec, size + 1);  // Trigger crash here
}

int main()
{
	OuterFunction(10);
	return 0;	
}
