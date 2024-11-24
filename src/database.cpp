#pragma once
#include "database.h"
#include "debug.h"
#include "models/customer.h"
#include "models/simple_models.h"
//#include "models/device.h"


soci::session Database::sql;

///// <summary>
///// If Database exists, opens it. Otherwise creates one, and opens it.
///// </summary>
///// <returns>Returns true if database could be open, false otherwise.  </returns>
bool Database::OpenDb() {
  try {
    // If already connected, return success
    if (sql.is_connected()) {
      return true;
    }

    // Open new connection
    sql.open(soci::sqlite3, "resource/reparo.db");
    is_initialized = true;
    Log::msg("DB IS OPEN");
    return true;
  }
  catch (const soci::soci_error& e) {
    std::cerr << "Cannot open database: " << e.what() << std::endl;
    is_initialized = false;
    return false;
  }
}

bool Database::Execute(const std::string& _sql) {
  try {
    sql << _sql;
    sql.close();
    return true;
  }
  catch (const std::exception& e) {
    std::cerr << "Execute failed, rolling back" << e.what() << std::endl;
    std::cerr << "Query: " << _sql << std::endl;
    sql.close();
    return false;
  }
}

bool Database::ExecuteTransaction(const std::string& _sql) {
  try {
    soci::transaction tran(sql);
    sql << _sql;

    tran.commit();
    sql.close();
    return true;
  }
  catch (const std::exception& e) {
    std::cerr << "Transaction failed, rolling back" << e.what() << std::endl;
    std::cerr << "Query: " << _sql << std::endl;
    sql.close();
    return false;
  }
}

TableCreator Database::Create() {
  if (!OpenDb()) {
    Log::msg("Failed to open database");
  }
  return TableCreator();
}

Inserter Database::Insert() {
    return Inserter();
}

Updater Database::Update() {
  return Updater();
}

Deleter Database::Delete() {
    return Deleter();
}

TableCreator& TableCreator::BillingAddressesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS billing_addresses (
      id         INTEGER PRIMARY KEY,               
      line1      TEXT,                              
      line2      TEXT,                              
      line3      TEXT,                              
      line4      TEXT,                              
      line5      TEXT,                              
      created_at DATETIME DEFAULT CURRENT_TIMESTAMP 
    );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::ShipAddressesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS ship_addresses (
      id         INTEGER PRIMARY KEY,
      line1      TEXT,         
      line2      TEXT,                
      line3      TEXT,
      line4      TEXT,
      line5      TEXT,
      created_at DATETIME DEFAULT CURRENT_TIMESTAMP
    );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::CustomersTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS customers (
      id                  INTEGER PRIMARY KEY,
      phone               TEXT NOT NULL UNIQUE,         
      name                TEXT,                
      surname             TEXT,
      email               TEXT,
      billing_addr_id     INTEGER,
      ship_addr_id        INTEGER,
                
      FOREIGN KEY (billing_addr_id)
        REFERENCES billing_addresses(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE,

      FOREIGN KEY (ship_addr_id)
        REFERENCES ship_addresses(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE
      );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::SuppliersTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS suppliers (
      id         INTEGER PRIMARY KEY,
      supplier   TEXT NOT NULL UNIQUE,
      line1      TEXT,         
      line2      TEXT,                
      line3      TEXT,
      line4      TEXT,
      line5      TEXT         
    );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairStatesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repair_states (
      id         INTEGER PRIMARY KEY,
      state   TEXT NOT NULL UNIQUE        
    );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::ColorsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS colors (
      id        INTEGER PRIMARY KEY,
      color    TEXT NOT NULL UNIQUE         
    );
  )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairCategoriesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repair_categories (
      id          INTEGER PRIMARY KEY,
      category  TEXT NOT NULL UNIQUE         
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::DeviceTypesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS device_types (
        id         INTEGER PRIMARY KEY,
        type       TEXT NOT NULL UNIQUE         
      );
    )";

  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::BrandsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS brands (
      id      INTEGER PRIMARY KEY,
      brand   TEXT NOT NULL UNIQUE         
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::QualitiesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS qualities (
      id         INTEGER PRIMARY KEY,
      quality   TEXT NOT NULL UNIQUE         
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PaymentMethodsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS payment_methods (
      id         INTEGER PRIMARY KEY,
      method     TEXT NOT NULL UNIQUE         
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::InventoryActionsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS inventory_actions (
      id         INTEGER PRIMARY KEY,
      action     TEXT NOT NULL UNIQUE         
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::DevicesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS devices (
      id        INTEGER PRIMARY KEY,
      model     TEXT UNIQUE NOT NULL,
      brand_id  INTEGER,
      type_id   INTEGER,

      FOREIGN KEY (brand_id)
        REFERENCES brands(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE,  
 
      FOREIGN KEY (type_id)
        REFERENCES device_types(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE       
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::CustomDevicesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS custom_devices (
      id        INTEGER PRIMARY KEY,
      model     TEXT NOT NULL,
      brand     TEXT,
      type      TEXT
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::AliasesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS aliases (
      id        INTEGER PRIMARY KEY,
      alias     TEXT NOT NULL UNIQUE,
      model_id     INTEGER NOT NULL,    

      FOREIGN KEY (model_id)
        REFERENCES devices(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE     
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}


TableCreator& TableCreator::ModelColorsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS model_colors (
      model_id     TEXT NOT NULL,
      color_id     TEXT NOT NULL,

      FOREIGN KEY (model_id)
        REFERENCES devices(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE, 
     
      FOREIGN KEY (color_id)
        REFERENCES colors(id)
        ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PartsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS parts (
      id                  INTEGER PRIMARY KEY,
      name                TEXT NOT NULL,
      own_sku             TEXT NOT NULL UNIQUE,
      quality_id          INTEGER,
      sell_price          REAL,
      sell_price_ex_vat   REAL,
      color_id            INTEGER,
      quantity            INTEGER DEFAULT 0,
      purch_price         REAL,
      purch_price_ex_vat  REAL,
      location            TEXT,
      reserved_quantity   INTEGER DEFAULT 0,
      created_at          DATETIME DEFAULT CURRENT_TIMESTAMP,
      updated_at          DATETIME DEFAULT CURRENT_TIMESTAMP,

      FOREIGN KEY (quality_id)
        REFERENCES qualities(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE, 
     
      FOREIGN KEY (color_id)
        REFERENCES colors(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PurchaseInvoicesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS purchase_invoices (
      id                 INTEGER PRIMARY KEY,
      invoice_number     TEXT NOT NULL,
      supplier_id        INTEGER,
      vat_rate           REAL NOT NULL,
      invoice_date       DATE NOT NULL,
      created_at         DATETIME DEFAULT CURRENT_TIMESTAMP,
    
      FOREIGN KEY (supplier_id)
          REFERENCES suppliers(id)
          ON DELETE SET NULL
          ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PurchaseInvoicesItemsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS purchase_invoice_items (
    id                      INTEGER PRIMARY KEY,
    purchase_invoice_id     TEXT NOT NULL,
    part_id                 INTEGER,
    supplier_sku            TEXT,
    temp_part_name          TEXT,  -- Used when part doesn't exist yet
    own_sku                 TEXT,  -- Can be used to link to future part
    purchase_price          REAL NOT NULL,
    purchase_price_ex_vat   REAL NOT NULL,
    quantity                INTEGER NOT NULL,
    created_at              DATETIME DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (purchase_invoice_id)
        REFERENCES purchase_invoices(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE,
        
    FOREIGN KEY (part_id)
        REFERENCES parts(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE
);

  -- Trigger to create new part when own_sku is inserted in purchase_invoice_items
  CREATE TRIGGER IF NOT EXISTS create_new_part_from_purchase_item
      AFTER INSERT ON purchase_invoice_items
      WHEN NEW.part_id IS NULL
  BEGIN
      INSERT INTO parts (name, own_sku)
      VALUES (NEW.supplier_sku, NEW.own_sku);
    
      UPDATE purchase_invoice_items
      SET part_id = (SELECT id FROM parts WHERE own_sku = NEW.own_sku)
      WHERE id = NEW.id;
  END;

  -- Trigger to update parts.updated_at
  CREATE TRIGGER IF NOT EXISTS update_part_timestamp
      AFTER UPDATE ON parts
  BEGIN
      UPDATE parts 
      SET updated_at = CURRENT_TIMESTAMP
      WHERE id = NEW.id;
  END;

  -- Trigger to update parts quantity after purchase
  CREATE TRIGGER IF NOT EXISTS after_purchase_item_insert
      AFTER INSERT ON purchase_invoice_items
      WHEN NEW.part_id IS NOT NULL
  BEGIN
      UPDATE parts 
      SET quantity = quantity + NEW.quantity,
          last_purchase_price = NEW.purchase_price,
          last_purchase_price_ex_vat = NEW.purchase_price_ex_vat
      WHERE id = NEW.part_id;
  END;


  -- Trigger to update parts history after purchase
  CREATE TRIGGER IF NOT EXISTS update_part_history
      AFTER INSERT ON purchase_invoice_items
  BEGIN
      INSERT INTO parts_history (part_id, operation, quantity, action_id, notes, created_at)
      VALUES  (NEW.part_id, "Add", NEW.quantity, 1, NEW.purchase_invoice_id, CURRENT_TIMESTAMP)
  END;
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PartsHistoryTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS parts_history (
      id                 INTEGER PRIMARY KEY,
      part_id            INTEGER NOT NULL,
      operation          TEXT,  -- Add / Deduct
      quantity           INTEGER NOT NULL,
      action_id          INTEGER NOT NULL,
      notes              TEXT NOT NULL,
      created_at         DATETIME DEFAULT CURRENT_TIMESTAMP,
    
      FOREIGN KEY (part_id)
          REFERENCES parts(id)
          ON DELETE CASCADE
          ON UPDATE CASCADE,

      FOREIGN KEY (action_id)
          REFERENCES inventory_actions(id)
          ON DELETE CASCADE
          ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repairs (
      id                 INTEGER PRIMARY KEY,
      customer_id        INTEGER NOT NULL,
      model_id           INTEGER DEFAULT NULL,
      category_id        INTEGER DEFAULT NULL,
      color_id           INTEGER DEFAULT NULL,
      visible_desc       TEXT,
      hidden_desc        TEXT,
      price              REAL NOT NULL,
      repair_state_id    INTEGER DEFAULT 1,
      sn_imei            TEXT,
      cust_device_id     INTEGER DEFAULT NULL,
      created_at         DATE DEFAULT CURRENT_TIMESTAMP,
      finished_at        DATE,

    
      FOREIGN KEY (customer_id)
          REFERENCES customers(id)
          ON UPDATE CASCADE,

      FOREIGN KEY (model_id)
          REFERENCES devices(id)
          ON UPDATE CASCADE,

      FOREIGN KEY (category_id)
          REFERENCES repair_categories(id)
          ON UPDATE CASCADE,

      FOREIGN KEY (color_id)
          REFERENCES colors(id)
          ON UPDATE CASCADE,

      FOREIGN KEY (repair_state_id)
          REFERENCES repair_states(id)
          ON UPDATE CASCADE,

      FOREIGN KEY (cust_device_id)
          REFERENCES custom_devices(id)
          ON UPDATE CASCADE

);
 -- Trigger to create new custom device when new device is inserted in repair
  CREATE TRIGGER IF NOT EXISTS create_new_custom_device
      AFTER INSERT ON repairs
      WHEN NEW.model_id IS NULL
  BEGIN
      INSERT INTO custom_device (model, brannd, color, )
      VALUES (NEW.model, NEW.brand, NEW.color);
  END;
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairPartsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repair_parts (
      repair_id          INTEGER,
      part_id            INTEGER,
      quantity           INTEGER NOT NULL,

      FOREIGN KEY (repair_id)
          REFERENCES repairs(id)
          ON DELETE CASCADE
          ON UPDATE CASCADE,

      FOREIGN KEY (part_id)
          REFERENCES parts(id)
          ON DELETE CASCADE
          ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::InvoicesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS invoices (
      id                  INTEGER PRIMARY KEY,
      invoice_num         INTEGER NOT NULL UNIQUE,
      repair_id           INTEGER,
      created_at          DATE DEFAULT CURRENT_TIMESTAMP,
      payment_method_id   INTEGER,
      paid                REAL,
      left_to_pay         REAL,

      FOREIGN KEY (repair_id)
          REFERENCES repairs(id),

      FOREIGN KEY (payment_method_id)
          REFERENCES payment_methods(id)
          ON UPDATE CASCADE
    );

    CREATE TRIGGER IF NOT EXISTS auto_increment_invoice_num
    AFTER INSERT ON invoices
    WHEN new.invoice_num IS NULL
    BEGIN
        UPDATE invoices 
        SET invoice_num = (SELECT COALESCE(MAX(invoice_num), 0) + 1 FROM invoices)
        WHERE id = new.id;
    END;
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PartModelTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS part_model (
      part_id             INTEGER NOT NULL,
      model_id            INTEGER NOT NULL,

      FOREIGN KEY (part_id)
          REFERENCES parts(id),

      FOREIGN KEY (model_id)
          REFERENCES devices(id)
          ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PartModelAliasTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS part_model_alias (
      part_id             INTEGER NOT NULL,
      alias_id            INTEGER NOT NULL,

      FOREIGN KEY (part_id)
          REFERENCES parts(id),

      FOREIGN KEY (alias_id)
          REFERENCES aliases(id)
          ON UPDATE CASCADE
    );
  )";
  Database::ExecuteTransaction(_sql);
  return *this;
}

Inserter& Inserter::Customer_(const Customer& customer) {
  return ExecuteTransaction(
    [&customer]() {
      int billing_addr_id = 0;
      if (!customer.billing_addresses.Get().Lines().empty()) {
        const auto& billing = customer.billing_addresses.Get().Lines(); // Using first address
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

      // Then insert shipping address and get its ID
      int shipping_addr_id = 0;
      if (!customer.ship_addresses.Get().Lines().empty()) {
        const auto& shipping = customer.ship_addresses.Get().Lines(); // Using first address
        Database::sql << "INSERT INTO ship_addresses "
          "(line1, line2, line3, line4, line5) "
          "VALUES (:l1, :l2, :l3, :l4, :l5) "
          "RETURNING id",
          soci::use(shipping[0]),
          soci::use(shipping[1]),
          soci::use(shipping[2]),
          soci::use(shipping[3]),
          soci::use(shipping[4]),
          soci::into(shipping_addr_id);
      }

      Database::sql << "INSERT INTO customers "
        "(phone, name, surname, email, billing_addr_id, ship_addr_id) "
        "VALUES (:phone, :name, :surname, :email, :bid, :sid)",
        soci::use(customer),
        soci::use(billing_addr_id, "bid"),
        soci::use(shipping_addr_id, "sid");
    },
    "Customer insertion (Phone: " + customer.phone + ")"
  );
}

Inserter& Inserter::Device_(Device& device) {
  return ExecuteTransaction(
    [&device]() {
      int device_id = 0;

      // 1. Insert main device data and get its ID
      Database::sql << "INSERT INTO devices (model, brand_id, type_id) VALUES (:model, :brand_id, :type_id) "
        "RETURNING id",
        soci::use(device), soci::into(device_id);

      // Set the device's ID
      device.id = device_id;

      // 2. Insert aliases
      for (const auto& alias : device.aliases) {
        Database::sql << "INSERT INTO aliases (alias, model_id) VALUES (:alias, :model_id)"
          "RETURNING id",
          soci::use(alias.name), soci::use(device.id);
      }

      // 3. Handle colors
      for (const auto& color : device.colors) {
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
        Database::sql << "INSERT INTO model_colors (model_id, color_id) VALUES (:model_id, :color_id)",
          soci::use(device_id), soci::use(color_id);
      }
    },
    "Device insertion (Device Name: " + device.name + ")"
  );
}