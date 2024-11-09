#pragma once
#include "text_fields.h"

class CustomerWin {
public:
  CustomerWin();
  void Render();

private:
  PhoneField phone;
  NameField name;
  SurnameField surname;
  EmailField email;
  bool open = true;
};