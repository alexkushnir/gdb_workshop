#include "Processor.h"

inline Processor<double, 20> p2Global1;
inline Processor<float, 25> p2Global2;

static inline void Proc2Func()
{
    p2Global1.process(3.14);
    p2Global2.process(2.718f);
}
