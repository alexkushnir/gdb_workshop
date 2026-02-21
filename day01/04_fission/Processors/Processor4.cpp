#include "Processor.h"

inline Processor<unsigned, 40> p4_global1;
inline Processor<long double, 12> p4_global2;

static inline void proc4_func() { 
    p4_global1.process(100U); 
    p4_global2.process(1.414L); 
}
