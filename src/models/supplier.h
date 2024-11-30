#pragma once
#include <string>
#include "address.h"

class Supplier {
public:
  Supplier();
  Supplier& GetSupplier();
  const std::string ToString() const;
  void View();

  int id = -1;
  std::string name;

  Address address;
  static constexpr std::string_view table = "suppliers";

};