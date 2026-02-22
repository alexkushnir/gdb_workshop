#include "Processor.h"

inline Processor<char, 5> p3_global1;
inline Processor<short, 8> p3_global2;

static inline void proc3_func()
{
    p3_global1.process('A');
    p3_global2.process(42);
}
