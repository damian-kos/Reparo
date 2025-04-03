#pragma once
#include <unordered_map>
#include "../../src/models/simple_models.h"
#include "../../src/models/supplier.h"

struct Device;
struct Alias;
struct InvoiceItem;

class Part {
public:
  const std::string ToString() const;
  int id = -1;
  std::string name;
  std::string own_sku;
  Quality quality;
  Category category;
  Supplier supplier;
  double sell_price = 0.0;
  double sell_price_ex_vat = 0.0;
  Color color;
  int quantity = 0;
  double purch_price = 0.0;
  double purch_price_ex_vat = 0.0;
  double vat = 0.0;
  std::string location;
  int reserved_quantity = 0;
  std::unordered_map<int, Device> cmptble_devices;
  std::unordered_map<int, Alias> cmptble_aliases;
  std::tm created_at = {};
  std::tm updated_at = {};
  static constexpr std::string_view table = "parts";
  static inline const std::string column = "own_sku";

  void View();
  void InsertModal();
  void InsertToDb();
  void UpdateModal();

  operator bool() const {
    return id > 0;
  }

  Part& operator=(InvoiceItem& _item);

private:

};