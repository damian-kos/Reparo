#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>
#include <vector>
#include <sstream>

class TableCreator;

//class SQLTransaction {
//private:
//  soci::session db;
//  bool active;
//
//public:
//  SQLTransaction(soci::session& db);
//  ~SQLTransaction();
//
//  bool Begin();
//  bool Commit();
//  bool Rollback();
//
//};
//
//
class Database {
private:
  static soci::session sql;

public:
  static bool OpenDb();
  //static bool TranaExecute(const std::string& _sql);
  static bool Execute(const std::string& _sql);
  static bool ExecuteTransaction(const std::string& _sql);
  static TableCreator Create();
//  static sqlite3* GetDb() { return db; }
//
//  static bool CreateDatabase();
};
//
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
  TableCreator& CustomDevicesTable();
  TableCreator& AliasesTable();
  TableCreator& ModelColorsTable();
  TableCreator& PartsTable();
  TableCreator& PurchaseInvoicesTable();
  TableCreator& PurchaseInvoicesItemsTable();
  TableCreator& PartsHistoryTable();
  TableCreator& RepairsTable();
  TableCreator& RepairPartsTable();
  TableCreator& InvoicesTable();
  TableCreator& PartModelTable();
  TableCreator& PartModelAliasTable();
};