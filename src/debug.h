#include <iostream>

namespace Log {
  void msg(const std::string& msg) {
    std::cout << msg << std::endl;
  }
}