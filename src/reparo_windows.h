#pragma once
#include "text_fields.h"

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