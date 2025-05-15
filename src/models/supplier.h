#pragma once
#include <string>
#include "address.h"

class Supplier {
public:
  Supplier();
  Supplier& GetSupplier();
  const std::string ToString(std::string _delimiter = " | ") const;
  const char* GetForCell() const;
  void View();
  void InsertToDb();
  void Clear();
  void UpdateModal();
  int id = -1;
  std::string name; // in database refered as 'supplier'

  Address address;
  static constexpr std::string_view table = "suppliers";
  // It has more columns but for simple retreive we can just use one
  static inline const std::string column = "supplier"; 
  operator bool() const {
    return id > 0 && !name.empty();
  }
};