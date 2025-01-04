#include "filters.h"
#include "imguidatechooser.h"
#include <iostream>
#include "conversion.h"
#include "RoLocalization.h"

DateFilter::DateFilter() 
  : directions{
    { 1, "Before"},
    { 2, "After"},
    { 3, "Between"}
  }
  {
  ImGui::SetDateZero(&from);
  ImGui::SetDateZero(&to);
}

bool DateFilter::Render() {
  bool _state = false;
  ImGui::PushItemWidth(250);
  if (ImGui::BeginCombo("##Relation", directions[selected].c_str())) {
    for (auto& pair : directions) {
      if (ImGui::Selectable(pair.second.c_str())) {
        selected = pair.first;
        _state = true;
      }
    }
    ImGui::EndCombo();
  }
  ImGui::SameLine();
  if (ImGui::DateChooser("##From", from)) {
    _state = true;
    std::cout << Convert::TmToStr(from) << std::endl;
  }
  ImGui::SameLine();
  ImGui::BeginDisabled(selected != 3);
  if (ImGui::DateChooser("##To", to)) {
    _state = true;
    std::cout << Convert::TmToStr(to) << std::endl;
  }
  ImGui::EndDisabled();
  ImGui::PopItemWidth();
  return _state;
}

std::string DateFilter::GetForSQL() {
  std::string _string_for_sql;
  switch (selected) {
    case 1:
      _string_for_sql = " < '" + Convert::TmToStr(from) + "'";
      break;
    case 2:
      _string_for_sql = " > '" + Convert::TmToStr(from) + "'";
      break;
    case 3:
      _string_for_sql = " BETWEEN '" + Convert::TmToStr(from) + "' AND '" + Convert::TmToStr(to) + "'";
      break;
    default:
      _string_for_sql = "";
      break;
  }
  return _string_for_sql;
}

ItemPicker::ItemPicker()
  : supplier_sku(_("Supplier SKU"), 0, TFFlags_HasPopup | TFFlags_NeverAnError, "DISTINCT supplier_sku", "purchase_invoice_items", "supplier_sku")
  , own_sku_field(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError)
  , name_field(_("Item's name"), 0, TFFlags_HasPopup | TFFlags_HasLenValidator | TFFlags_EmptyIsError, "DISTINCT name", "parts", "name")
{
}

void ItemPicker::Render() {
  supplier_sku.Render();
  own_sku_field.Render();
  name_field.Render();

  bool refresh = false;
  static int _quantity = 0;
  static double _price = 0.0;
  static double _vat = 0.0;
  static double _total_net = 0.0;
  static double _total = 0.0;

  if (ImGui::InputInt(_("Quantity"), &_quantity))
    refresh = true;
  if (ImGui::InputDouble(_("Purchase price ex. VAT"), &_price, 0.0, 0.0, "%.2f"))
    refresh = true;
  if (ImGui::InputDouble(_("VAT rate"), &_vat, 0.0, 0.0, "%.2f")) 
    refresh = true;
  
  ImGui::InputDouble(_("Total net: %.2f"), &_total_net, 0.0, 0.0, "%.2f");
  ImGui::InputDouble(_("Total: %.2f"), &_total, 0.0, 0.0, "%.2f");

  if (refresh) {
    _total_net = _quantity * _price;
    _total = _total_net + (_total_net * _vat / 100);
  }
}
