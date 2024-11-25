#pragma once
#include <string>
#include <vector>
#include "simple_models.h"

struct Alias {
public:
  Alias();
  Alias(int id, std::string name, int link_id = 0);

  int id = -1;        // main id in database
  std::string name;
  int link_id = -1;   // linked id in database in this case device
  static constexpr std::string_view table = "aliases";
  static inline const std::string column = "alias";

};