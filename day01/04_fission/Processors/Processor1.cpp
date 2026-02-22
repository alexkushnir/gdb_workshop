#include "Processor.h"

// Force instantiations + unique data
inline Processor<int, 10> p1_global;
inline Processor<double, 20> p2_global; // Change types/N for each file
static inline void proc1_func()
{
    p1_global.process(1);
}
