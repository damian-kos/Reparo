#pragma once
#include <string>
#include <vector>
#include "model_handler.h"

struct DeviceColor {
  int id = -1;        // main id in database
  int link_id = -1;   // linked id in database in this case color_id
  static constexpr std::string_view table = "model_colors";
};

