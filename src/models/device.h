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

  operator bool() const {
    return id > 0;
  }

  bool operator==(const Device& other) const {
    if (id != other.id)
      return false;
    return name == other.name&&
      brand == other.brand &&
      type == other.type;

  }
};

struct CustomDevice {
  int id = -1;
  std::string name;
  Color color;
  void EditModal();
  void DeleteModal();
  static constexpr std::string_view table = "custom_devices";

};