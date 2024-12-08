#pragma once
#include "../../src/models/simple_models.h"
#include "../../src/models/alias.h"
#include <string>
#include <vector>

struct Device {
  int id = -1;
  std::string name;
  Brand brand;
  DeviceType type;
  std::vector<Color> colors;
  std::vector<Alias> aliases;
  static constexpr std::string_view table = "devices";

  const std::string ToString() const;
};

struct CustomDevice {
  int id = -1;
  std::string name;
  Color color;
  static constexpr std::string_view table = "custom_devices";

};