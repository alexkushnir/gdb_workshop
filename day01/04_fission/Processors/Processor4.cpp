#include "Processor.h"

inline Processor<unsigned, 40> p4Global1;
inline Processor<long double, 12> p4Global2;

static inline void Proc4Func()
{
    p4Global1.Process(100U);
    p4Global2.Process(1.414L);
}
