#pragma once
#include "../../src/models/simple_models.h"

class Part {
public:
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
  std::string created_at; // date
  std::string updated_at; // date

private:

};