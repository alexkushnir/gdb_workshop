#include "Processor.h"

inline Processor<double, 20> p2_global1;
inline Processor<float, 25> p2_global2;

static inline void proc2_func() { 
    p2_global1.process(3.14); 
    p2_global2.process(2.718f); 
}
