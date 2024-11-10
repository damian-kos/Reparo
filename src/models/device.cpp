#include "device.h"
#include <iostream>

Device::Device() { }

Device& Device::GetDevice() {
  std::cout << "Device: " << id << " / " << name << " / " << brand << " / " << type << " / " << colors[0] << std::endl;
  return *this;
}
