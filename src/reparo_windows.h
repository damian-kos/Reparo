#pragma once
#include "text_fields.h"
#include "models/simple_models.h"
#include "RoLocalization.h"
#include "tables.h"
#include "database.h"

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

class DeviceWin {
public:
  DeviceWin();
  void Render();

private:
  TextField name;
  TextField brand;
  TextField type;
  TextField color;
  TextField alias;
  bool open = true;

};
};

template <typename T>
class SimpleModelWin {
public:
  SimpleModelWin();
  void Render();
  void LoadData();

private:
  T model;
  TextField name;
  std::vector<T> values;
  bool open = true;
  bool initialized = false;
};

template<typename T>
inline SimpleModelWin<T>::SimpleModelWin() { }

template<typename T>
inline void SimpleModelWin<T>::Render(){
  if (!initialized) {
    LoadData();
    initialized = true;
  }
  ImGui::Begin(model.Get<T>().WindowTitle().c_str(), &open);
  RoTable::SimpleModel<T>(values);
  name.Render();
  ImGui::Text(_("Please right-click to edit or delete value"));
  if (ImGui::Button(_("Add"))) {
   model.Set<T>()
      .Name(name.Get());
    Database::Insert().OfSimpleModel<T>(model);
    LoadData();
    model.Get<T>().Table();
  }
  ImGui::End();
}

template<typename T>
inline void SimpleModelWin<T>::LoadData() {
  values = Database::Select<T>().From().All();
}
