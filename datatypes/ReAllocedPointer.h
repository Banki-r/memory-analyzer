#pragma once
#include <iostream>

struct ReAllocedPointer {
  std::string name;
  std::string allocLine;
  std::string allocFunc;
  std::string reAllocedLine;
};