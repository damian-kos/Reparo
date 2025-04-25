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
            quality_id, category_id, supplier_id, sell_price, sell_price_ex_vat, color_id, 
            quantity, purch_price, purch_price_ex_vat, vat, location) 
            VALUES (:name, :own_sku, 
            :quality_id, :category_id, :supplier_id, :sell_price, :sell_price_ex_vat, :color_id, 
            :quantity, :purch_price, :purch_price_ex_vat, :vat, :location) 
            RETURNING id)",
      soci::use(_p), soci::into(_part_id);
    _part.id = _part_id;
    return _part_id;
  }

  int UpdateItem(Part& _part) {
    Part _p = _part;
    int _part_id = _p.id;
    Database::sql << R"(UPDATE parts 
            SET name = :name, own_sku = :own_sku, quality_id = :quality_id, category_id = :category_id, 
            supplier_id = :supplier_id, sell_price = :sell_price, sell_price_ex_vat = :sell_price_ex_vat, 
            color_id = :color_id, quantity = :quantity, purch_price = :purch_price, purch_price_ex_vat = :purch_price_ex_vat, 
            vat = :vat, location = :location 
            WHERE id = :id)",
      soci::use(_p);
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

  int UpdateRepair(Repair& repair)  {
    Repair r = repair;
    int _repair_id = r.id; 
    Database::sql << R"(UPDATE repairs 
            SET customer_id = :customer_id, model_id = :model_id, category_id = :category_id, 
            color_id = :color_id, visible_desc = :visible_desc, hidden_desc = :hidden_desc, 
            price = :price, repair_state_id = :repair_state_id, sn_imei = :sn_imei, cust_device_id = :cust_device_id 
            WHERE id = :id)",
      soci::use(r);

    if (!r.last_update.empty()) {
      Database::sql << R"(INSERT INTO repair_updates (repair_id, note) 
              VALUES (:repair_id, :note))",
        soci::use(_repair_id),
        soci::use(r.last_update);
    }
    return _repair_id;
  }

  int UpdateRepairPart(RepairItem& _item) {
    RepairItem _i = _item;
    Database::sql << "UPDATE repair_parts SET "
      "quantity = :quantity, "
      "sell_price_ex_vat = :sell_price_ex_vat, "
      "vat = :vat "
      "WHERE repair_id = :repair_id AND part_id = :part_id",
      soci::use(_i);
    return _i.id;
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
    Device _d = repair.device;
    int _device_id = 0;

    Database::sql << "SELECT id FROM custom_devices WHERE model = :model AND color = :color",
      soci::use(_d.name),
      soci::use(_d.colors[0].name),
      soci::into(_device_id);

    if (_device_id == 0) { // If not found, insert
      Database::sql << "INSERT INTO custom_devices (model, color) "
        "VALUES (:model, :color) "
        "RETURNING id",
        soci::use(_d.name),
        soci::use(_d.colors[0].name),
        soci::into(_device_id);
    }
    repair.cust_device_id = _device_id;
    return _device_id;
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

  void UpdateColors(UpdateSet<Color>& _colors, int _model_id)   {
    for (auto& color : _colors.to_insert) {
      int color_id = 0;

      // Try to find existing color
      Database::sql << "SELECT id FROM colors WHERE color = :color",
        soci::use(color.name), soci::into(color_id);

      if (!color_id) {
        // Color doesn't exist, insert new color
        Database::sql << "INSERT INTO colors (color) VALUES (:color) RETURNING id",
          soci::use(color.name), soci::into(color_id);
      }

      // Insert into model_colors junction table
      Database::sql << R"(INSERT INTO model_colors 
          (model_id, color_id) VALUES (:model_id, :color_id))",
        soci::use(_model_id), soci::use(color_id);
    }

    // Delete colors
    for (auto& color : _colors.to_delete) {
      Database::sql << R"(DELETE FROM model_colors WHERE 
          color_id = :cid AND model_id = :mid )",
        soci::use(color.id), soci::use(_model_id);

      // Check if the color_id exists in any other model_colors row
      int count = 0;
      Database::sql << R"(SELECT COUNT(*) FROM model_colors WHERE color_id = :cid)",
        soci::use(color.id), soci::into(count);

      // If the color is no longer associated with any model, delete it from colors
      if (count == 0) {
        Database::sql << R"(DELETE FROM colors WHERE id = :cid)",
          soci::use(color.id);
      }
    }
  }

  void UpdateAliases(UpdateSet<Alias>& _aliases, int _model_id)   {
    // Insert aliases
    for (const auto& alias : _aliases.to_insert) {
      Database::sql << "INSERT INTO aliases (alias, model_id) VALUES (:alias, :mid)",
        soci::use(alias.name), soci::use(_model_id);
    }
    // Delete aliases
    for (const auto& alias : _aliases.to_delete) {
      Database::sql << "DELETE FROM aliases WHERE alias = :alias AND model_id = :mid",
        soci::use(alias.name), soci::use(_model_id);
    }
  }

  void UpdateDevice(Device& _device) {
    Device _d = _device;

    Database::sql << R"(UPDATE devices
      SET model = :model, brand_id = :brand_id, type_id = :type_id
      WHERE id = :id)",
      soci::use(_d);
  }

  void SetTriggerContext(const int& _context) {
    Database::sql << "UPDATE trigger_context SET trigger = :context WHERE id = 1",
      soci::use(_context);
  }

  void ResetTriggerContext() {
    Database::sql << "UPDATE trigger_context SET trigger = 0 WHERE id = 1";
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