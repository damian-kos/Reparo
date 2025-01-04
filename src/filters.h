#pragma once
#include <unordered_map>
#include <string>
#include "text_fields.h"
class DateFilter {
public:
  DateFilter();
  bool Render();
  std::string GetForSQL();
private:
  tm from;
  tm to;
  std::unordered_map<int, std::string> directions;
  int selected = 1;
  bool disabled = true;
};

class ItemPicker {
public:
  ItemPicker();
  void Init();
  void Render();
private:
  QueriedTextField supplier_sku;
  OwnSKUField own_sku_field;
  QueriedTextField name_field; // for the name of the part
};