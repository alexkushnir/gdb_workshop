#include "Processor.h"

inline Processor<char, 5> p3Global1;
inline Processor<short, 8> p3Global2;

static inline void Proc3Func()
{
    p3Global1.Process('A');
    p3Global2.Process(42);
}
