#pragma once
#include "simple_models.h"
#include <string>
#include <vector>

struct Alias {
public:
  Alias();
  Alias(int id, std::string name, int link_id = 0);

  int id = -1;        // main id in database
  std::string name;
  int link_id = -1;   // linked id in database in this case device
  static constexpr std::string_view table = "aliases";
  static inline const std::string column = "alias";
  const std::string ToString() const;

  operator bool() const {
    return id > 0 && !name.empty() && link_id > 0;
  }

  bool operator<(const Alias& other) const {
    return name < other.name; // Ensure uniqueness is determined correctly
  }
};