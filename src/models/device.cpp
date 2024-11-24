#include "device.h"
#include <iostream>

const std::string Device::ToString() const{
  return std::to_string(id) + " | " + name + " | " + brand.name + " | " + type.name;
}
