#pragma once
#include <unordered_map>
#include <string>
#include "text_fields.h"
#include "invoices.h"

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
};

class ItemPicker {
public:
  ItemPicker();
  void Init();
  void Render();
  void FillFields(Part& _part);
  InvoiceItem GetPart(); // Return the selected part
  void Clear();
  bool error = true;

private:
  int quantity = 0;
  double price = 0.0;
  double vat = 0.0;
  double total_net = 0.0;
  double total = 0.0;
  QueriedTextField supplier_sku;
  OwnSKUField own_sku_field;
  QueriedTextField name_field; // for the name of the part
  void Validate();
};