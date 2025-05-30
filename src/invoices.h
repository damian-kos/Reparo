#pragma once
#include "../../src/models/parts.h"
#include "../../src/models/supplier.h"

#include "../../src/models/simple_models.h"
#include "../../src/conversion.h"
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
  Price price;
  const std::string ToString() const;
  void Clear();

  bool operator==(const RepairItem& other) const {
    return (part.id == other.part.id &&
      quantity == other.quantity &&
      total_net == other.total_net &&
      total == other.total);
  }
};

template <typename T>
class ItemsContainer {
public:
  std::vector<T> records;
  Price total; // total price of all items

  void PushUnique(const T& _item) {
    Convert::PushBackIfUnique(records, _item);
    total.amount += _item.total;
  }

  void Push(const T& _item) {
    records.push_back(_item);
    total.amount += _item.total;
  }

  template <typename Iterator> 
  void Erase(Iterator& it) {
    if (it != records.end()) {
      total.amount -= it->price.amount;
      it = records.erase(it);
    }
  }

  void Clear() {
    records.clear();
    total.amount = 0;
  }

  void CalcTotal() {
    total.amount = 0;
    for (const auto& item : records) {
      total.amount += item.price.amount;
    }
  }
};

class Invoice {
public:
  int id = -1;
  std::string name; // invoice number / id / name
  tm purchased_at;
  tm arrived_at;
  tm created_at;
  virtual void InsertToDb() {};
  virtual void View();
};

class PurchaseInvoice : public Invoice {
public:
  std::string external_id;
  Supplier supplier;
  const std::string ToString() const;
  void View() override;
  void InsertToDb() override;
  void InsertModal();
  void UpdateModal();
  void UpdateToDb();
  ItemsContainer<InvoiceItem> items;
  static constexpr std::string_view table = "purchase_invoices";
  static inline const std::string column = "invoice_number";
  operator bool() const {
    return id > 0 && !name.empty();
  }
};