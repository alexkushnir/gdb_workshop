#include "Processor.h"

// Force instantiations + unique data
inline Processor<int, 10> p1Global;
inline Processor<double, 20> p2Global; // Change types/N for each file
static inline void Proc1Func()
{
    p1Global.Process(1);
}
