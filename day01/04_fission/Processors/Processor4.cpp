#include "Processor.h"

inline Processor<unsigned, 40> p4Global1;
inline Processor<long double, 12> p4Global2;

static inline void Proc4Func()
{
    p4Global1.process(100U);
    p4Global2.process(1.414L);
}
