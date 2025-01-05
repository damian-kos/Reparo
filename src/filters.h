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
  struct PartInvoice {
    std::string name;
    std::string supplier_sku;
    std::string own_sku;
    int quantity;
    double price; // ex_vat
    double vat;
    double total_net;
    double total;
    Part part;
  };
  PartInvoice GetPart(); // Return the selected part
private:
  int quantity = 0;
  double price = 0.0;
  double vat = 0.0;
  double total_net = 0.0;
  double total = 0.0;
  QueriedTextField supplier_sku;
  OwnSKUField own_sku_field;
  QueriedTextField name_field; // for the name of the part
};