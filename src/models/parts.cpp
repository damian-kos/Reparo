#include "parts.h"
#include "../../src/RoLocalization.h"
#include "../../src/modal.h"
#include "../../src/database.h"
#include "../../src/tables.h"
#include "../../src/invoices.h"
#include "../../src/LocStrings.h"
#include "../../src/conversion.h"

const std::string Part::ToString() const{
  return std::to_string(id) + " | " + name + " | " + own_sku;
}

std::vector<ModelDataView> Part::CreateViewData(const Part& _previous) {
  bool _comp = _previous.id > 0;
  std::vector<ModelDataView> _view_data;
  _view_data.push_back({ LBL_SKU, own_sku, _comp ? own_sku != _previous.own_sku : false});
  _view_data.push_back({ LBL_NAME, name, _comp ? name != _previous.name : false});
  _view_data.push_back({ LBL_QUALITY, quality.name, _comp ? quality.name != _previous.quality.name : false});
  _view_data.push_back({ LBL_CATEGORY, category.name, _comp ? category.name != _previous.category.name : false});
  _view_data.push_back({ LBL_COLOR, color.name, _comp ? color != _previous.color : false});
  _view_data.push_back({ LBL_QUANTITY, std::to_string(quantity), _comp ? quantity != _previous.quantity : false});
  _view_data.push_back({ LBL_PURCHASE_PRICE, Convert::to_string_2dp(purch_price), _comp ? purch_price != _previous.purch_price : false});
  _view_data.push_back({ LBL_EX_VAT, Convert::to_string_2dp(purch_price_ex_vat), _comp ? purch_price_ex_vat != _previous.purch_price_ex_vat : false});
  _view_data.push_back({ LBL_VAT, Convert::to_string_2dp(vat), _comp ? vat != _previous.vat : false});
  _view_data.push_back({ LBL_SELL_PRICE, Convert::to_string_2dp(sell_price), _comp ? sell_price != _previous.sell_price : false});
  _view_data.push_back({ LBL_EX_VAT, Convert::to_string_2dp(sell_price_ex_vat), _comp ? sell_price_ex_vat != _previous.sell_price_ex_vat : false});
  _view_data.push_back({ LBL_LOCATION, location, _comp ? location != _previous.location : false});
  return _view_data;
}

void Part::View() {
  RoTable::TableWrapped(device_entries);
  RoTable::TableWrapped(alias_entries);
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

void Part::UpdateToDb() {
  Database::Update().Part_(*this);
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

const char* Part::GetLocationForCell() const {
  return location.empty() ? UNKOWN : location.c_str();
}
