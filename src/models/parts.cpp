#include "parts.h"
#include "../../src/RoLocalization.h"
#include "../../src/modal.h"
#include "../../src/database.h"
#include "../../src/tables.h"
#include "../../src/invoices.h"
#include "../../src/LocStrings.h"

const std::string Part::ToString() const{
  return std::to_string(id) + " | " + name + " | " + own_sku;
}

void Part::View() { 
  ImGui::Separator();
  ImGui::SeparatorText(_("Item"));
  ImGui::Text("%s:", LBL_SKU); ImGui::SameLine(); ImGui::Text("%s", own_sku.c_str());
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

void Part::View(const Part& _previous) const {
  ImGui::Separator();
  ImGui::SeparatorText(_("Item"));
  ImGui::TextWrappedColor(own_sku != _previous.own_sku,                       "Own SKU:        %s", own_sku.c_str());
  ImGui::TextWrappedColor(name != _previous.name,                             "Name:           %s", name.c_str());
  ImGui::TextWrappedColor(quality.name != _previous.quality.name,             "Quality:        %s", quality.name.c_str());
  ImGui::TextWrappedColor(category.name != _previous.category.name,           "Category:       %s", category.name.c_str());
  ImGui::TextWrappedColor(color.name != _previous.color.name,                 "Color:          %s", color.name.c_str());
  ImGui::TextWrappedColor(quantity != _previous.quantity,                     "Quantity:       %d", quantity);
  ImGui::TextWrappedColor(purch_price != _previous.purch_price,               "Purchase price: %.2f", purch_price);
  ImGui::TextWrappedColor(purch_price_ex_vat != _previous.purch_price_ex_vat, "Ex. VAT:        %.2f", purch_price_ex_vat);
  ImGui::TextWrappedColor(vat != _previous.vat, "VAT:            %.2f", vat);
  ImGui::TextWrappedColor(sell_price != _previous.sell_price, "Sell Price:     %.2f", sell_price);
  ImGui::TextWrappedColor(sell_price_ex_vat != _previous.sell_price_ex_vat, "Ex. VAT:        %.2f", sell_price_ex_vat);
  ImGui::TextWrappedColor(location != _previous.location, "Location:       %s", location.c_str());
}

void Part::InsertModal() {
  ModalConfig _config;
  _config.Title(_("Insert new item?"))
    .State(ModalState_Insert);;
  PartModal _modal(*this, _config);
  StackModal::SetModal(_modal);
}

void Part::InsertToDb() { 
  Database::Insert().Part_(*this); 
}

void Part::UpdateModal() {
  ModalConfig _config;
  _config.Title(_("Update item?"))
    .State(ModalState_UpdateWindow);
  PartModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

Part& Part::operator=(InvoiceItem& _item) {
  own_sku = _item.own_sku;
  name = _item.name;
  purch_price = _item.price.amount;
  purch_price_ex_vat = _item.price.ExVat();
  vat = _item.price.vat_rate;
  quantity = _item.quantity;
  return *this;
}
