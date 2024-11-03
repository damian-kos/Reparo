#pragma once
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <sstream>

class TableCreator;

class SQLTransaction {
private:
  sqlite3* db;
  bool active;

public:
  SQLTransaction(sqlite3* db);
  ~SQLTransaction();

  bool EnableForeignKeys();
  bool Begin();
  bool Commit();
  bool Rollback();

};


class Database {
private:
  static sqlite3* db;
  static bool OpenDb();

public:
  static bool Execute(const std::string& _sql);
  static TableCreator Create();
  static sqlite3* GetDb() { return db; }

  static bool CreateDatabase();
};

class TableCreator {
public:
  TableCreator() = default;
  TableCreator& BillingAddressesTable();
  TableCreator& ShipAddressesTable();
  TableCreator& CustomersTable();
  TableCreator& SuppliersTable();
  TableCreator& RepairStatesTable();
  TableCreator& ColorsTable();
  TableCreator& RepairCategoriesTable();
  TableCreator& DeviceTypesTable();
  TableCreator& BrandsTable();
  TableCreator& QualitiesTable();
  TableCreator& PaymentMethodsTable();
  TableCreator& InventoryActionsTable();
  TableCreator& DevicesTable();
  TableCreator& AliasesTable();
  TableCreator& ModelColorsTable();

private:
  bool ExecuteWithTransaction(const std::string& _sql);
};