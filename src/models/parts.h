#pragma once
#include "../../src/models/simple_models.h"

class Part {
public:
  const std::string ToString() const;
  int id = -1;
  std::string name;
  std::string own_sku;
  Quality quality;
  double sell_price;
  double sell_price_ex_vat;
  Color color;
  int quantity;
  double purch_price;
  double purch_price_ex_vat;
  std::string location;
  int reserved_quantity;
  std::tm created_at; // date
  std::tm updated_at; // date
  static constexpr std::string_view table = "parts";
  static inline const std::string column = "own_sku";

private:

};