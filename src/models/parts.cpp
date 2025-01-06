#include "parts.h"
#include "../../src/RoLocalization.h"
#include "../../src/modal.h"
#include "../../src/database.h"
#include "../../src/tables.h"

const std::string Part::ToString() const{
  return std::to_string(id) + " | " + name + " | " + own_sku;
}

void Part::View() { 
  ImGui::Separator();
  ImGui::SeparatorText(_("Item"));
  ImGui::Text("%s:", _("Own SKU        ")); ImGui::SameLine(); ImGui::Text("%s", own_sku.c_str());
  ImGui::Text("%s:", _("Name           ")); ImGui::SameLine(); ImGui::Text("%s", name.c_str());
  ImGui::Text("%s:", _("Quality        ")); ImGui::SameLine(); ImGui::Text("%s", quality.name.c_str());
  ImGui::Text("%s:", _("Category       ")); ImGui::SameLine(); ImGui::Text("%s", category.name.c_str());
  ImGui::Text("%s:", _("Color          ")); ImGui::SameLine(); ImGui::Text("%s", color.name.c_str());
  ImGui::Text("%s:", _("Quantity       ")); ImGui::SameLine(); ImGui::Text("%d", quantity);
  ImGui::Text("%s:", _("Purchase price ")); ImGui::SameLine(); ImGui::Text("%.2f", purch_price);
  ImGui::Text("%s:", _("Ex. VAT        ")); ImGui::SameLine(); ImGui::Text("%.2f", purch_price_ex_vat);
  ImGui::Text("%s:", _("VAT            ")); ImGui::SameLine(); ImGui::Text("%.2f", vat);
  ImGui::Text("%s:", _("Sell Price     ")); ImGui::SameLine(); ImGui::Text("%.2f", sell_price);
  ImGui::Text("%s:", _("Ex. VAT        ")); ImGui::SameLine(); ImGui::Text("%.2f", sell_price_ex_vat);
  ImGui::Text("%s:", _("Location       ")); ImGui::SameLine(); ImGui::Text("%s", location.c_str());
  RoTable::TableWrapped(cmptble_devices);
  RoTable::TableWrapped(cmptble_aliases);
}

void Part::InsertModal() {
  ModalConfig _config;
  _config.Title(_("Insert new item?"));
  PartModal _modal(*this, _config);
  StackModal::SetModal(_modal);
}

void Part::InsertToDb() { 
  Database::Insert().Part_(*this); 
}
