#pragma once
#include "text_fields.h"
#include "models/simple_models.h"

class Brand;

class CustomerWin {
public:
  CustomerWin();
  void Render();
  void Addresses();

private:
  PhoneField phone;
  NameField name;
  SurnameField surname;
  EmailField email;
  std::vector<TextField> billing_address;
  std::vector<TextField> ship_address;
  bool open = true;
};

class BrandWin {
public:
  BrandWin();
  void Render();
  
private:
  TextField name;
  std::vector<Brand> brands;
  bool open = true;
};