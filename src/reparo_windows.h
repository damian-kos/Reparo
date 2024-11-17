#pragma once
#include "text_fields.h"
#include "models/simple_models.h"
#include "RoLocalization.h"
#include "tables.h"
#include "database.h"
#include "combo.h"
#include "attributes.h"

class Brand;

class CustomerWin {
public:
  CustomerWin();
  void Render();

private:
  void Addresses();
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
  void DeviceName();

private:
  TextField name;
  RoCombo<Brand> brand_combo;
  RoCombo<DeviceType> type_combo;
  Attributes<Color> colors;
  Attributes<Alias> aliases;
  bool open = true;
};

class RepairWin {
public:
  RepairWin();
  void Render();

private:
  CustomerWin customer_part;

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
inline void SimpleModelWin<T>::Render() {
  if (!initialized) {
    LoadData();
    initialized = true;
  }
  ImGui::Begin(model.window_title.c_str(), &open);
  RoTable::SimpleModel<T>(values);
  name.Render();
  ImGui::Text(_("Please right-click to edit or delete value"));
  if (ImGui::Button(_("Add"))) {
    model.name = name.Get();
    Database::Insert().OfSimpleModel<T>(model);
    LoadData();
    model.table;
  }
  ImGui::End();
}

template<typename T>
inline void SimpleModelWin<T>::LoadData() {
  std::string text = Database::Select<T>().From().GetSql();
  std::cout << text << std::endl;
  values = Database::Select<T>().From().All();
}