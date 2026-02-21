#include <iostream>
#include "Processors/Processor.h"
#include "Utils/Heavy.h"

int main() {
    Processor<int, 10> p1;
    Processor<double, 20> p2;
    Processor<char, 5> p3;
    Processor<float, 30> p4;

    p1.process(42); p1.process(100);
    p2.process(3.14); p2.process(2.718);
    p3.process('A'); p3.process('B');
    p4.process(1.414f); p4.process(0.577f);

    p1.dump(); p2.dump(); p3.dump(); p4.dump();

    HeavyUtil util;
    util.heavy_compute();

    return 0;
}
