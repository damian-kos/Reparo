#pragma once
#include "../../src/models/parts.h"
#include "../../src/models/supplier.h"

#include "../../src/models/simple_models.h"
#include <string>

struct InvoiceItem {
  int id = -1;
  int purchase_invoice_id;
  Part part;
  std::string supplier_sku;
  std::string name;
  std::string own_sku;
  Price price;
  double total_net;
  double total;
  int quantity;

  const std::string ToString() const;
};

class Invoice {
public:
  int id = -1;
  std::string name; // invoice number / id / name
  tm purchased_at;
  tm arrived_at;
  tm created_at;

};

class PurchaseInvoice : public Invoice {
public:
  std::string external_id;
  Supplier supplier;
  const std::string ToString() const;
  void InsertToDb();
  std::vector<InvoiceItem> items;
  static constexpr std::string_view table = "purchase_invoices";
  static inline const std::string column = "invoice_number";
  operator bool() const {
    return id > 0 && !name.empty();
  }
};