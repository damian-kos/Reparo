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

class InvoiceItemPicker {
public:
  InvoiceItemPicker();
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

// Shorter names for the flags from now on
enum ItemFilterFlags_ {
  DisableNone      = 0, 
  DisableOwnSKU    = 1 << 1,
  DisableName      = 1 << 2,
  DisableDevice    = 1 << 3,
  DisableAlias     = 1 << 4,
  DisableQuality   = 1 << 5,
  DisableCategory  = 1 << 6,
  ShowTable        = 1 << 7
};

typedef int ItemFilterFlags;

class ItemFilter {
public:
  ItemFilter(ItemFilterFlags _flags = 0);
  bool Render();
  std::string GetOwnSKU();
  std::string GetName();
  std::string GetDevice();
  std::string GetAlias();
  std::string GetQuality();
  std::string GetCategory();
private:
  ItemFilterFlags flags = 0;
  OwnSKUField own_sku_filter;
  QueriedTextField name_filter;
  DeviceField device_filter;
  SimpleModelField<Alias> alias_filter;
  QueriedTextField quality_filter;
  QueriedTextField category_filter;
};