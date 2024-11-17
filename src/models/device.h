#pragma once
#include <string>
#include <vector>
#include "simple_models.h"
#include "alias.h"

struct Device {
  int id = -1;
  std::string name;
  Brand brand;
  DeviceType type;
  std::vector<Color> colors;
  std::vector<Alias> aliases;
  static constexpr std::string_view table = "devices";
};