#pragma once
#include "../../src/models/parts.h"
#include "../../src/models/supplier.h"

#include "../../src/models/simple_models.h"
#include <string>

struct InvoiceItem {
  int purchase_invoice_id;
  Part part;
  std::string supplier_sku;
  std::string name;
  std::string own_sku;
  Price price;
  double total_net;
  double total;
  int quantity;
};


class Invoice {
public:
  int id;
  std::string number;
  tm created_at;
  tm inserted_at;
};

class PurchaseInvoice : public Invoice {
public:

private:
  Supplier supplier;
};