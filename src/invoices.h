#pragma once
#include "../../src/models/parts.h"
#include "base_window.h"
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
};

class PurchaseInvoice : public Invoice {
public:

private:

};