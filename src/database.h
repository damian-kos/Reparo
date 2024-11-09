#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>
#include <vector>
#include <sstream>

class TableCreator;
class Inserter;
class Customer;

class Database {
public:
  static soci::session sql;
  static bool OpenDb();
  static bool Execute(const std::string& _sql);
  static bool ExecuteTransaction(const std::string& _sql);
  static TableCreator Create();
  static Inserter Insert();

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


class Inserter {
public:
  Inserter() = default;
  Inserter& Customer_(const Customer& customer);

private:
  template<typename Func>
  Inserter& ExecuteTransaction(Func operation, const std::string& description);
};

template<typename Func>
inline Inserter& Inserter::ExecuteTransaction(Func operation, const std::string& description) {
  try {
    Database::OpenDb();
    soci::transaction tr(Database::sql);
    operation();
    tr.commit();
    std::cout << "Successfully completed: " << description << std::endl;
    Database::sql.close();
    return *this;
  }
  catch (const soci::sqlite3_soci_error& e) {
    std::cerr << "SQLite error in " << description << ": " << e.what() << std::endl;
    Database::sql.close();
  }
  catch (const soci::soci_error& e) {
    std::cerr << "General SOCI error in " << description << ": " << e.what() << std::endl;
    Database::sql.close();
  }
  catch (const std::exception& e) {
    std::cerr << "Unexpected error in " << description << ": " << e.what() << std::endl;
    Database::sql.close();
  }
  return *this;
}
