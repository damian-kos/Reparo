#include "supplier.h"
#include "imgui.h"
#include "../../src/RoLocalization.h"

Supplier::Supplier() { }

Supplier& Supplier::GetSupplier() {
  return *this;
}

const std::string Supplier::ToString() const {
  return std::to_string(id) + " | " + name;
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
