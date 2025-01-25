#pragma once
#include "../../src/models/parts.h"
#include "../../src/models/supplier.h"

#include "../../src/models/simple_models.h"
#include <string>

struct SelectableItem {
  int id = -1;
  Part part;
  int quantity = 0;
};

struct InvoiceItem : public SelectableItem {
  std::string name;
  int purchase_invoice_id;
  std::string supplier_sku;
  std::string own_sku;
  Price price;
  double total_net;
  double total;

  const std::string ToString() const;
};

struct RepairItem : public SelectableItem {
  int repair_id = -1;
  bool assign = false; // assigns to repair reserved parts
  double total_net;
  double total;
  const std::string ToString() const;
  void Clear();
};

template <typename T>
class ItemsContainer {
public:
  std::vector<T> records;
  Price total; // total price of all items
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

