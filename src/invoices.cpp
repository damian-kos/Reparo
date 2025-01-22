#include "invoices.h"
#include "RoLocalization.h"
#include "conversion.h"
#include "database.h"

const std::string InvoiceItem::ToString() const {
  return std::to_string(id) + " | " + name + " | " + supplier_sku + " | " + own_sku + " | " + part.ToString();;
}

const std::string PurchaseInvoice::ToString() const {
  return std::to_string(id) + " | Ext. ID " + external_id + " | " + name + " | Purchased at: " + Convert::TmToStr(purchased_at) + " | Supplier " + supplier.ToString();
}

void PurchaseInvoice::InsertToDb() {
  Database::Insert().PurchaseInvoice_(*this);
}

const std::string RepairItem::ToString() const {
  return std::to_string(id) + " | " + part.name + " | " + std::to_string(part.sell_price) + " | " + part.ToString();;
}

void RepairItem::Clear() {
  id = -1;
  part = Part();
  quantity = 0;
  assign = false;
}
