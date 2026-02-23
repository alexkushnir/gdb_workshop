#include "Processors/Processor.h"
#include "Utils/Heavy.h"
#include <iostream>

int main()
{
    Processor<int, 10> p1;
    Processor<double, 20> p2;
    Processor<char, 5> p3;
    Processor<float, 30> p4;

    p1.Process(42);
    p1.Process(100);
    p2.Process(3.14);
    p2.Process(2.718);
    p3.Process('A');
    p3.Process('B');
    p4.Process(1.414f);
    p4.Process(0.577f);

    p1.Dump();
    p2.Dump();
    p3.Dump();
    p4.Dump();

    HeavyUtil util;
    util.HeavyCompute();

    return 0;
}
