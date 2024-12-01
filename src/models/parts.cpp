#include "parts.h"

const std::string Part::ToString() const{
  return std::to_string(id) + " | " + name + " | " + own_sku;
}
