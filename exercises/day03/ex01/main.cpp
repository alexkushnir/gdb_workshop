//======================================================
// bad thread games...
// @author Amir Kirsh
//======================================================

#include <iostream>
#include <thread>

void func(int a, double* sum) {
    for(long i=0; i<100; ++i) {
        *sum += a;
    }
}

int main(){
    double sum = 0;
    std::thread t[10];
    size_t size = sizeof(t)/sizeof(t[0]);
    for(size_t i=0; i<size; ++i) {
        t[i] = std::thread(func, i, &sum);
    }
    for(size_t i=0; i<size; ++i) {
        t[i].join();
    }
    std::cout << sum << std::endl;
}
