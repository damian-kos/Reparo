#pragma once
#include <vector>
#include "text_fields.h"

class RoTable {
public:
  static void AddressesInputs(std::vector<TextField>& first, std::vector<TextField>& second);
  static void Addresses(std::vector<std::string>& first, std::vector<std::string>& second);
};

