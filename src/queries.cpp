#include "queries.h"
#include "database.h"
#include "../../src/models/customer.h"

/// <summary>
/// Used to allow to run multiple queries in one Transaction without need to reopening database.
/// </summary>
namespace Query {
  int InsertBillingAddress(Customer& customer) {
    Customer c = customer;
    int billing_addr_id = 0;
    if (!c.billing_addresses.Get().Lines().empty()) {
      const auto& billing = c.billing_addresses.Get().Lines(); // Using first address
      Database::sql << "INSERT INTO billing_addresses "
        "(line1, line2, line3, line4, line5) "
        "VALUES (:l1, :l2, :l3, :l4, :l5) "
        "RETURNING id",
        soci::use(billing[0]),
        soci::use(billing[1]),
        soci::use(billing[2]),
        soci::use(billing[3]),
        soci::use(billing[4]),
        soci::into(billing_addr_id);
    }
    customer.billing_addresses.SetID(billing_addr_id);
    return billing_addr_id;
  }

  int UpdateBillingAddress(Customer& customer) {
    Customer c = customer;
    int billing_addr_id = c.billing_addresses.Get().ID();
    if (!c.billing_addresses.Get().Lines().empty()) {
      const auto& billing = c.billing_addresses.Get().Lines(); // Using first address
      Database::sql << "UPDATE billing_addresses "
        "SET line1 = :l1, line2 = :l2, line3 = :l3 , line4 = :l4, line5 = :l5 "
        "WHERE id = :id "
        "RETURNING id",
        soci::use(billing[0]),
        soci::use(billing[1]),
        soci::use(billing[2]),
        soci::use(billing[3]),
        soci::use(billing[4]),
        soci::use(billing_addr_id);
    }
    return billing_addr_id;
  }

  int InsertShippingAddress(Customer& customer) {
    Customer c = customer;
    int ship_addr_id = 0;
    if (!c.ship_addresses.Get().Lines().empty()) {
      const auto& ship = c.ship_addresses.Get().Lines(); // Using first address
      Database::sql << "INSERT INTO ship_addresses "
        "(line1, line2, line3, line4, line5) "
        "VALUES (:l1, :l2, :l3, :l4, :l5) "
        "RETURNING id",
        soci::use(ship[0]),
        soci::use(ship[1]),
        soci::use(ship[2]),
        soci::use(ship[3]),
        soci::use(ship[4]),
        soci::into(ship_addr_id);
    }
    customer.ship_addresses.SetID(ship_addr_id);
    return ship_addr_id;
  }

  int UpdateShippingAddress(Customer& customer) {
    Customer c = customer;
    int ship_addr_id = c.ship_addresses.Get().ID();
    if (!c.ship_addresses.Get().Lines().empty()) {
      const auto& ship = c.ship_addresses.Get().Lines(); // Using first address
      Database::sql << "UPDATE ship_addresses "
        "SET line1 = :l1, line2 = :l2, line3 = :l3 , line4 = :l4, line5 = :l5 "
        "WHERE id = :id "
        "RETURNING id",
        soci::use(ship[0]),
        soci::use(ship[1]),
        soci::use(ship[2]),
        soci::use(ship[3]),
        soci::use(ship[4]),
        soci::use(ship_addr_id);
    }
    return ship_addr_id;
  }

  int InsertCustomer(Customer& customer) {
    Customer c = customer;
    int customer_id = -1;
    Database::sql << "INSERT INTO customers "
      "(phone, name, surname, email, billing_addr_id, ship_addr_id) "
      "VALUES (:phone, :name, :surname, :email, :billing_addr_id, :ship_addr_id) "
      "RETURNING id",
      soci::use(c),
      soci::into(customer_id);
    customer.id = customer_id;
    return customer_id;
  }

  int UpdateCustomer(Customer& customer) {
    Customer c = customer;
    int customer_id = c.id;
    Database::sql << "UPDATE customers "
      "SET phone = :phone, name = :name, surname = :surname, email = :email, billing_addr_id = :billing_addr_id, ship_addr_id = :ship_addr_id "
      "WHERE id = :id "
      "RETURNING id",
      soci::use(c);
    return customer_id;
  }

  int InsertItem(Part& _part)  {
    Part _p = _part;
    int _part_id = -1;
    Database::sql << R"(INSERT INTO parts (name, own_sku, 
            quality_id, category_id, sell_price, sell_price_ex_vat, color_id, 
            quantity, purch_price, purch_price_ex_vat, vat, location) 
            VALUES (:name, :own_sku, 
            :quality_id, :category_id, :sell_price, :sell_price_ex_vat, :color_id, 
            :quantity, :purch_price, :purch_price_ex_vat, :vat, :location) 
            RETURNING id)",
      soci::use(_p), soci::into(_part_id);
    _part.id = _part_id;
    return _part_id;
  }

  int UpdateItem(InvoiceItem& _item) {
    InvoiceItem _i = _item;
    int _item_id = _i.id;
    int _new_quantity = _item.part.quantity + _item.quantity;
    Database::sql << R"(UPDATE parts 
            SET purch_price = :purch_price, purch_price_ex_vat = :purch_price_ex_vat, 
            quantity = :quantity, vat = :vat 
            WHERE id = :id)",
      soci::use(_i.price.amount),
      soci::use(_i.price.ExVat()),
      soci::use(_new_quantity),
      soci::use(_i.price.vat_rate),
      soci::use(_i.part.id);
    return _item_id;
  }

  void InsertItemDevices(Part& _part) {
    Part _p = _part;
    for (auto& [key, value] : _p.cmptble_devices) {
      Database::sql << R"(INSERT INTO part_model (part_id, model_id)
            VALUES (:part_id, :model_id))",
        soci::use(_part.id),
        soci::use(value.id);
    }
  }

  void InsertItemAliases(Part& _part) {
    Part _p = _part;
    for (auto& [key, value] : _p.cmptble_aliases) {
      Database::sql << R"(INSERT INTO part_model_alias (part_id, alias_id)
            VALUES (:part_id, :alias_id))",
        soci::use(_part.id),
        soci::use(value.id);
    }
  }

  int InsertRepair(Repair& repair) {
    Repair r = repair;
    int repair_id = 0;
    Database::sql << "INSERT INTO repairs (customer_id, model_id, category_id, color_id, visible_desc, hidden_desc, price, repair_state_id, sn_imei, cust_device_id) "
      "VALUES (:customer_id, :model_id, :category_id, :color_id, :visible_desc, :hidden_desc, :price, :repair_state_id, :sn_imei, :cust_device_id) "
      "RETURNING id",
      soci::use(r),
      soci::into(repair_id);
    repair.id = repair_id;
    return repair_id;
  }

  int InsertCustomDevice(Repair& repair) {
    // We are using device but in fact we will be instering custom device
    Device d = repair.device;
    int device_id = 0;
    Database::sql << "INSERT INTO custom_devices (model, color) "
      "VALUES (:model, :color) "
      "RETURNING id",
      soci::use(d.name),
      soci::use(d.colors[0].name),
      soci::into(device_id);
    repair.cust_device_id = device_id;
    return device_id;
  }

  void InsertRepairPart(RepairItem& _item) {
    RepairItem _i = _item;
    Database::sql << "INSERT INTO repair_parts (repair_id, part_id, quantity, sell_price_ex_vat, vat) "
      "VALUES (:repair_id, :part_id, :quantity, :sell_price_ex_vat, :vat)",
      soci::use(_i);
  }

  int InsertSupplier(Supplier& _supplier)   {
    Supplier _s = _supplier;
    int _supplier_id = -1;
    Database::sql << "INSERT INTO suppliers "
      "(supplier, line1, line2, line3, line4, line5) "
      "VALUES (:supplier ,:line1 ,:line2 ,:line3 ,:line4 ,:line5) "
      "RETURNING id",
      soci::use(_s),
      soci::into(_supplier_id);
    std::cout << _supplier_id << std::endl;
    _supplier.id = _supplier_id;
    return _supplier_id;
  }

  int InsertPurchaseInvoice(PurchaseInvoice& _invoice) {
    PurchaseInvoice _i = _invoice;
    int _invoice_id = -1;
    Database::sql << "INSERT INTO purchase_invoices "
      "(invoice_number, external_id, supplier_id, purchased_at, arrived_at, created_at) "
      "VALUES (:invoice_number, :external_id, :supplier_id, :purchased_at, :arrived_at, :created_at) "
      "RETURNING id",
      soci::use(_i),
      soci::into(_invoice_id);
    _invoice.id = _invoice_id;
    return _invoice_id;
  }

  int InsertInvoiceItem(InvoiceItem& _item) {
    InvoiceItem _i = _item;
    int _item_id = -1;

    Database::sql << "INSERT INTO purchase_invoice_items "
      "(purchase_invoice_id, part_id, supplier_sku, name, own_sku, purchase_price, purchase_price_ex_vat, quantity, vat) "
      "VALUES (:purchase_invoice_id, :part_id, :supplier_sku, :name, :own_sku, :purchase_price, :purchase_price_ex_vat, :quantity, :vat) "
      "RETURNING id",
      soci::use(_i),
      soci::into(_item_id);
    _item.id = _item_id;
    return _item_id;
  }

  template <typename T>
  int InsertSimpleModel(T& _model) {
    // for safety of object lifetime and immutability
    T _m = _model;
    int _model_id = 0;

    std::string column = " (" + _m.column + ") ";
    std::string value = "(:" + _m.column + ")";
    Database::sql << "INSERT INTO " << _m.table << column
      << "VALUES " << value << " RETURNING id"
      , soci::use(_m.name)
      , soci::into(_model_id);

    _model.id = _model_id;
    return _model_id;
  }

  // Explicit instantiations
  template int InsertSimpleModel<Brand>(Brand& _model);
  template int InsertSimpleModel<RepairState>(RepairState& _model);
  template int InsertSimpleModel<Category>(Category& _model);
  template int InsertSimpleModel<PaymentMethod>(PaymentMethod& _model);
  template int InsertSimpleModel<DeviceType>(DeviceType& _model);
  template int InsertSimpleModel<Color>(Color& _model);
  template int InsertSimpleModel<Quality>(Quality& _model);

}