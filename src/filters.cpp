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

InvoiceItemPicker::InvoiceItemPicker()
  : supplier_sku(_("Supplier SKU"), 0, TFFlags_HasPopup | TFFlags_NeverAnError, "DISTINCT supplier_sku", "purchase_invoice_items", "supplier_sku")
  , own_sku_field(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence)
  , name_field(_("Item's name"), 0, TFFlags_HasPopup | TFFlags_HasLenValidator | TFFlags_EmptyIsError, "DISTINCT name", "parts", "name")
{
}

void InvoiceItemPicker::Render() {
  own_sku_field.Feedback();
  name_field.FeedbackEx();
  ImGui::NewLine();

  bool refresh = false;

  if (supplier_sku.Render()) {
    if (supplier_sku.Get().empty()) { return; } 
    Part _part = Database::Select<Part>("p.*").From("purchase_invoice_items pii")
      .InnerJoin("parts p")
      .On("pii.part_id = p.id")
      .Where("supplier_sku", supplier_sku.Get())
      .One();
    if (!_part) { return; }
    FillFields(_part);
    refresh = true;
  }

  if (own_sku_field.Render()) {
    if (own_sku_field.Get().empty()) { return; }
    Part _part = own_sku_field.GetFromDb();
    if (!_part) { return; }
    FillFields(_part);
    refresh = true;
  }

  name_field.Render();

  if (ImGui::InputInt(_("Quantity"), &quantity))
    refresh = true;
  if (ImGui::InputDouble(_("Purchase price ex. VAT"), &price, 0.0, 0.0, "%.2f"))
    refresh = true;
  if (ImGui::InputDouble(_("% VAT rate"), &vat, 0.0, 0.0, "%.2f"))
    refresh = true;

  ImGui::InputDouble(_("Total net"), &total_net, 0.0, 0.0, "%.2f");
  ImGui::InputDouble(_("Total"), &total, 0.0, 0.0, "%.2f");

  if (refresh) {
    total_net = quantity * price;
    total = total_net + (total_net * vat / 100);
  }
  Validate();
}

void InvoiceItemPicker::FillFields(Part& _part) {
  own_sku_field.FillBuffer(_part.own_sku);
  name_field.FillBuffer(_part.name);
  price = _part.purch_price;
  vat = _part.vat;
  quantity = 1;
}

InvoiceItem InvoiceItemPicker::GetPart() {
  InvoiceItem _part;
  _part.part = own_sku_field.GetFromDb();
  _part.name = name_field.Get();
  _part.supplier_sku = supplier_sku.Get();
  _part.own_sku = own_sku_field.Get();
  _part.quantity = quantity;
  _part.price.amount = price;
  _part.price.vat_rate = vat;
  _part.total_net = total_net;
  _part.total = total;
  std::cout << _part.part.ToString() << std::endl;
  return _part;
}

void InvoiceItemPicker::Clear() {
  quantity = 1;
  price = 0.0;
  vat = 0.0;
  total_net = 0.0;
  total = 0.0;
  supplier_sku.Clear();
  own_sku_field.Clear();
  name_field.Clear();
}

void InvoiceItemPicker::Validate() {
  error = own_sku_field.error || name_field.error;
}

ItemFilter::ItemFilter(ItemFilterFlags _flags) 
  : flags(_flags)
  , own_sku_filter(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence)
  , name_filter(_("Item's name"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence, "name", "parts", "name")
  , device_filter(_("Compatible with device"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence)
  , alias_filter(_("Compatible with alias"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence)
  , quality_filter(_("Choose quality"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence, "quality", "qualities", "quality")
  , category_filter(_("Choose category"), 0, TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence, "category", "repair_categories", "category")
{
}

bool ItemFilter::Render() {
  bool _state = false;
  if (!(flags & DisableOwnSKU))
    own_sku_filter.Render();
  if (!(flags & DisableName))
    name_filter.Render();
  if (!(flags & DisableDevice))
    device_filter.Render();
  if (!(flags & DisableAlias))
    alias_filter.Render();
  if (!(flags & DisableQuality))
    quality_filter.Render();
  if (!(flags & DisableCategory))
    category_filter.Render();
  if (ImGui::Button(_("Search")))
    _state = true;
  return _state;
}

std::string ItemFilter::GetOwnSKU() {
  return own_sku_filter.Get();
}

std::string ItemFilter::GetName() {
  return name_filter.Get();
}

std::string ItemFilter::GetDevice() {
  Device _device = device_filter.GetFromDb();
  std::string _device_cmpbl = _device ? "pm.model_id = " + std::to_string(_device.id) : "";
  return _device_cmpbl;
}

std::string ItemFilter::GetAlias() {
  Alias _alias = alias_filter.GetFromDb();
  std::string _alias_cmpbl = _alias ? "pma.alias_id = " + std::to_string(_alias.id) : "";
  return _alias_cmpbl;
}

std::string ItemFilter::GetQuality() {
  return quality_filter.Get();
}

std::string ItemFilter::GetCategory() {
  return category_filter.Get();
}

RepairItemPicker::RepairItemPicker(RepairItem& _item) 
 : repair_item(_item)
{
}

void RepairItemPicker::Render() {
  ImGui::SeparatorText(_("Edit item"));
  ImGui::Text("Name: %s", repair_item.part.name.c_str()); // with TextWrapped the popup window is rendering in unexpected areas
  ImGui::InputInt(_("Quantity"), &repair_item.quantity);
  ImGui::InputDouble(_("Sell price ex. VAT"), &repair_item.part.sell_price, 0.0, 0.0, "%.2f"); 
  ImGui::SameLine();
  ImGui::Text(_(" - %d available"), repair_item.part.quantity);
  ImGui::InputDouble(_("% VAT"), &repair_item.part.vat, 0.0, 0.0, "%.2f");
  if (ImGui::Button(_("Assign part")))
    repair_item.assign = true;
}

RepairItem RepairItemPicker::GetItem() {
  return repair_item;
}

void RepairItemPicker::Clear() {
  repair_item = RepairItem();
}
