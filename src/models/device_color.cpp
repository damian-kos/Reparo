#include "device_color.h"

DeviceColor::DeviceColor() { }

DeviceColor::DeviceColor(int id, int link_id) : id(id), link_id(link_id) { }

DeviceColor& DeviceColor::GetDeviceColor() {
  return *this;
}
