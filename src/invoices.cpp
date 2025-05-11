#include "invoices.h"
#include "RoLocalization.h"
#include "conversion.h"
#include "database.h"
#include "modal.h"

const std::string InvoiceItem::ToString() const {
  return std::to_string(id) + " | " + name + " | " + supplier_sku + " | " + own_sku + " | " + part.ToString();;
}

void RepairItem::Clear() {
  id = -1;
  part = Part();
  quantity = 0;
  assign = false;
}

void Invoice::View() {
}

const std::string PurchaseInvoice::ToString() const {
  return std::to_string(id) + " | Ext. ID " + external_id + " | " + name + " | Purchased at: " + Convert::TmToStr(purchased_at) + " | Supplier " + supplier.ToString();
}

void PurchaseInvoice::View() {
}

void PurchaseInvoice::InsertToDb() {
  Database::Insert().PurchaseInvoice_(*this);
}

void PurchaseInvoice::InsertModal() {
  ModalConfig _config;
  _config.Title(LBL_PI_INSERT_TITLE)
    .Msg(TXT_PI_INSERT_MSG)
    .State(ModalState_Insert);
  ConfirmCancelModal _modal(*this, _config);
  StackModal::SetModal(_modal);
}

void PurchaseInvoice::UpdateModal() {
  ModalConfig _config;
  _config.Title(LBL_PI_EDIT_TITLE)
    .State(ModalState_UpdateWindow);
  PurchaseInvoiceModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

void PurchaseInvoice::UpdateToDb() {
}

const std::string RepairItem::ToString() const {
  return std::to_string(id) + " | " + part.name + " | " + std::to_string(part.sell_price) + " | " + part.ToString();;
}
