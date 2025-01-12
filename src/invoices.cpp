#include "invoices.h"
#include "RoLocalization.h"
#include "conversion.h"
#include "database.h"

const std::string InvoiceItem::ToString() const {
  return std::to_string(id) + " | " + name + " | " + supplier_sku + " | " + own_sku + " | " + part.ToString();;
}

const std::string PurchaseInvoice::ToString() const {
  return number + " | Purchased at: " + Convert::TmToStr(purchased_at) + " | Supplier " + supplier.ToString();
}

void PurchaseInvoice::InsertToDb() {
  Database::Insert().PurchaseInvoice_(*this);
}

