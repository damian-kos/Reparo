#include <array>
#include "supplier.h"
#include "imgui.h"
#include "../../src/RoLocalization.h"
#include "../../src/database.h"
#include "../../src/LocStrings.h"
#include "../../src/modal.h"

Supplier::Supplier() { }

Supplier& Supplier::GetSupplier() {
  return *this;
}

const std::string Supplier::ToString(const std::string _delimiter) const {
  std::string _str = std::to_string(id) + _delimiter + name;
  _str += address.ToString(_delimiter);
  return _str;
}

const char* Supplier::GetForCell() const {
  return name.empty() ? UNKOWN : name.c_str();
}

void Supplier::View() {
  if (id > 0) {
    ImGui::Text(_("Supplier ID: %d"), id);
  }
  ImGui::Text(_("Name: %s"), name.c_str());
  ImGui::SeparatorText(_("Address"));

  for (auto& line : address.Get().Lines()) {
    ImGui::Text("%s", line.c_str());
  }
}

std::vector<ModelDataView> Supplier::CreateViewData(const Supplier& _previous) const {
  bool _comp = _previous.id > 0;
  std::vector<ModelDataView> _view_data;
  _view_data.push_back({ LBL_SUPPLIER, name, _comp ? name != _previous.name : false });

  static const std::array<const char*, 5> _addr_labels = {
    SUPPLIER_ADDR_1,
    SUPPLIER_ADDR_2,
    SUPPLIER_ADDR_3,
    SUPPLIER_ADDR_4,
    SUPPLIER_ADDR_5
  };

  std::vector<std::string> _lines = address.Get().Lines();
  std::vector<std::string> _previous_lines = _previous.address.Get().Lines();

  for (size_t i = 0; i < _addr_labels.size(); ++i) {
    _view_data.push_back({ _addr_labels[i], _lines[i], _comp ? _lines[i] != _previous_lines[i] : false});
  }

  return _view_data;

}

void Supplier::InsertToDb() {
  Database::Insert().Supplier_(*this);
}

void Supplier::Clear() {
  id = -1;
  name.clear();
  address.Clear();
}

void Supplier::UpdateModal() {
  ModalConfig _config;
  _config.Title(LBL_SUPPLIER_EDIT)
    .State(ModalState_UpdateWindow);
  SupplierModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

void Supplier::UpdateToDb() {
  // update
}
