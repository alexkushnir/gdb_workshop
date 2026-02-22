#pragma once
#include "../Utils/Heavy.h"
#include <print>
#include <string>
#include <typeinfo>
#include <vector>

template <typename T, int N> class Processor
{
    std::vector<T> buffer;
    HeavyData data;

public:
    Processor() : data("Processor<" + std::string(typeid(T).name()) + "," + std::to_string(N) + ">")
    {
        buffer.reserve(N);
    }
    void process(T val)
    {
        buffer.push_back(val * T(N));
    }
    void dump()
    {
        std::print("Proc size: {}, data: {}\n", buffer.size(), data.name);
    }
};
