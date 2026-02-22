#pragma once
#include "../Utils/Heavy.h"
#include <print>
#include <string>
#include <typeinfo>
#include <vector>

template <typename T, int N>
class Processor
{
    std::vector<T> m_buffer;
    HeavyData m_data;

public:
    Processor()
        : m_data("Processor<" + std::string(typeid(T).name()) + "," + std::to_string(N) + ">")
    {
        m_buffer.reserve(N);
    }
    void Process(T val)
    {
        m_buffer.push_back(val * T(N));
    }
    void Dump()
    {
        std::print("Proc size: {}, data: {}\n", m_buffer.size(), m_data.name);
    }
};
