#pragma once
#include <string>

class AReallyImportantClass
{
public:
    AReallyImportantClass() = default;
    void DoSomethingVeryImportant();
    void DoOtherThingWhichIsVeryImportantToo();
    void DoSomethingWhichIsNotImportant();

private:
    int m_integerImportantData;
    double m_doubleImportantData;
    std::string m_stringImportantData;
};