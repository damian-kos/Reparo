#pragma once
#include "database.h"
#include "debug.h"

SQLTransaction::SQLTransaction(sqlite3* db) : db(db), active(false) {
  EnableForeignKeys();
  Begin();
}

SQLTransaction::~SQLTransaction() {
  if (active) {
    Rollback();
  }
}

bool SQLTransaction::Begin() {
  char* _err_msg = nullptr;
  if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &_err_msg) == SQLITE_OK) {
    active = true;
    return true;
  }
  else {
    std::cerr << "Failed to start transaction: " << _err_msg << std::endl;
    sqlite3_free(_err_msg);
    return false;
  }
}

bool SQLTransaction::EnableForeignKeys() {
  char* _err_msg = nullptr;
  if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &_err_msg) != SQLITE_OK) {
    std::cerr << "Failed to enable foreign key constraints: " << _err_msg << std::endl;
    sqlite3_free(_err_msg);
    return false;
  }
  return true;
}

bool SQLTransaction::Commit() {
  char* _err_msg = nullptr;
  if (sqlite3_exec(db, "COMMIT;", NULL, NULL, &_err_msg) == SQLITE_OK) {
    active = false;
    return true;
  }
  else {
    std::cerr << "Failed to Commit transaction: " << _err_msg << std::endl;
    sqlite3_free(_err_msg);
    return false;
  }
}

bool SQLTransaction::Rollback() {
  char* _err_msg = nullptr;
  if (sqlite3_exec(db, "ROLLBACK;", NULL, NULL, &_err_msg) == SQLITE_OK) {
    active = false;
    return true;
  }
  else {
    std::cerr << "Failed to Rollback transaction: " << _err_msg << std::endl;
    sqlite3_free(_err_msg);
    return false;
  }
}


sqlite3* Database::db = nullptr;

/// <summary>
/// If Database exists, opens it. Otherwise creates one, and opens it.
/// </summary>
/// <returns>Returns true if database could be open, false otherwise.  </returns>
bool Database::OpenDb() {
  int _rc = sqlite3_open("resource/reparo.db", &db);
  if (_rc) {
    std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }
  Log::msg("DB IS OPEN");;
  return true;
}

bool Database::Execute(const std::string& _sql) {
  char* _err_msg = nullptr;
  int _rc = sqlite3_exec(db, _sql.c_str(), nullptr, nullptr, &_err_msg);

  if (_rc != SQLITE_OK) {
    std::cerr << "SQL error: " << _err_msg << std::endl;
    sqlite3_free(_err_msg);
    return false;
  }
  return true;
}


bool Database::CreateDatabase() {
  if (OpenDb()) { 
    Log::msg("Database already exists");
    return false;  
  }
  return true;
}

TableCreator Database::Create() {
  if (!OpenDb()) {
    Log::msg("Failed to open database");
  }
  return TableCreator();
}

bool TableCreator::ExecuteWithTransaction(const std::string& _sql) {
  SQLTransaction transaction(Database::GetDb());
  transaction.EnableForeignKeys();

  if (!Database::Execute(_sql)) {
    transaction.Rollback();
    return false;
  }

  if (!transaction.Commit()) {
    transaction.Rollback();
    return false;
  }

  return true;
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

  ExecuteWithTransaction(_sql);
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

  ExecuteWithTransaction(_sql);
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
      line5               TEXT,
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

  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::SuppliersTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS suppliers (
      id         INTEGER PRIMARY KEY,
      supplier   TEXT NOT NULL UNIQUE         
    );
  )";

  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairStatesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repair_states (
      id         INTEGER PRIMARY KEY,
      state   TEXT NOT NULL UNIQUE        
    );
  )";

  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::ColorsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS colors (
      id        INTEGER PRIMARY KEY,
      color    TEXT NOT NULL UNIQUE         
    );
  )";

  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::RepairCategoriesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS repair_categories (
      id          INTEGER PRIMARY KEY,
      category  TEXT NOT NULL UNIQUE         
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::DeviceTypesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS device_types (
        id         INTEGER PRIMARY KEY,
        type       TEXT NOT NULL UNIQUE         
      );
    )";

  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::BrandsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS brands (
      id         INTEGER PRIMARY KEY,
      brand   TEXT NOT NULL UNIQUE         
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::QualitiesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS qualities (
      id         INTEGER PRIMARY KEY,
      quality   TEXT NOT NULL UNIQUE         
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::PaymentMethodsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS payment_methods (
      id         INTEGER PRIMARY KEY,
      method     TEXT NOT NULL UNIQUE         
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::InventoryActionsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS inventory_actions (
      id         INTEGER PRIMARY KEY,
      action     TEXT NOT NULL UNIQUE         
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::DevicesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS devices (
      id        INTEGER PRIMARY KEY,
      model     TEXT NOT NULL,
      brand     INTEGER,
      type      INTEGER,

      FOREIGN KEY (brand)
        REFERENCES brands(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE,  
 
      FOREIGN KEY (type)
        REFERENCES device_types(id)
        ON DELETE SET NULL
        ON UPDATE CASCADE       
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}

TableCreator& TableCreator::AliasesTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS aliases (
      id        INTEGER PRIMARY KEY,
      alias     TEXT NOT NULL UNIQUE,
      model     INTEGER NOT NULL,    

      FOREIGN KEY (model)
        REFERENCES models(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE     
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}


TableCreator& TableCreator::ModelColorsTable() {
  std::string _sql = R"(
    CREATE TABLE IF NOT EXISTS model_colors (
      model     TEXT NOT NULL,
      color     TEXT NOT NULL,

      FOREIGN KEY (model)
        REFERENCES devices(id)
        ON DELETE CASCADE
        ON UPDATE CASCADE, 
     
      FOREIGN KEY (color)
        REFERENCES colors(id)
        ON UPDATE CASCADE
    );
  )";
  ExecuteWithTransaction(_sql);
  return *this;
}