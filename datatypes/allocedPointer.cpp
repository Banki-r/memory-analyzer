#include <iostream>

struct AllocedPointer
{
    std::string allocLine;
    std::string name;
    std::string freeLine = nullptr;
};