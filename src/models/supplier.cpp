#include "supplier.h"
#include "imgui.h"
#include "../../src/RoLocalization.h"
#include "../../src/database.h"
#include "../../src/LocStrings.h"

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

void Supplier::InsertToDb() {
  Database::Insert().Supplier_(*this);
}

void Supplier::Clear() {
  id = -1;
  name.clear();
  address.Clear();
}

void Supplier::UpdateModal() {
  std::cout << "Update supplier" << std::endl;
}
