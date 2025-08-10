#pragma once
#include <iostream>

struct AllocedPointer {
  std::string name;
  std::string allocLine;
  std::string allocFunc;
  std::string freeLine;
};