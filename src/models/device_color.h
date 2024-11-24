#pragma once
#include <string>
#include <vector>

struct DeviceColor {
  int id = -1;        // main id in database
  std::string name;
  int link_id = -1;   // linked id in database in this case color_id
  static constexpr std::string_view table = "model_colors";
  //static inline const std::string column = "color";
  // v Most likely remove
  static inline const std::string join = "LEFT JOIN model_colors mc ON colors.id = mc.color_id";

};

