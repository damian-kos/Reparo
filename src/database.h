#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "models/simple_models.h"
#include "../../src/soci_type_conversion_helpers.h"

class TableCreator;
class Inserter;
class Customer;
class Brand;
class SimpleModel;
class Database;

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
  Inserter& Brand_(Brand& brand);

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

template<typename T>
class Selector {
public:
  Selector(const std::string& columns = "*") {
    sql << "SELECT " << columns;
  }

  Selector& From() {
    T temp;
    sql << " FROM " << temp.Get<T>().Table();
    return *this;
  }

  Selector& Where(const std::string& condition) {
    sql << " WHERE " << condition;
    return *this;
  }

  Selector& OrderBy(const std::string& column, const std::string& order = "ASC") {
    sql << " ORDER BY " << column << " " << order;
    return *this;
  }

  Selector& Limit(int count) {
    sql << " LIMIT " << count;
    return *this;
  }

  Selector& Offset(int offset) {
    sql << " OFFSET " << offset;
    return *this;
  }

  std::vector<T> All() {
    std::vector<T> results;
    try {
      Database::OpenDb();
      soci::rowset<T> rs = (Database::sql.prepare << sql.str());
      for (const auto& row : rs) {
        results.push_back(row);
      }
      Database::sql.close();
    }
    catch (const soci::soci_error& e) {
      std::cerr << "SOCI error executing query: " << e.what() << std::endl;
      Database::sql.close();
    }
    return results;
  }

  T One() {
    try {
      Database::OpenDb();
      T result;
      soci::statement st = (Database::sql.prepare << sql.str(), soci::into(result));
      st.execute();
      st.fetch();
      Database::sql.close();
      return result;
    }
    catch (const soci::soci_error& e) {
      std::cerr << "SOCI error executing query: " << e.what() << std::endl;
      Database::sql.close();
      throw;
    }
  }

  std::string GetSql() const {
    return sql.str();
  }

private:
  std::ostringstream sql;
};

class Database {
public:
  static soci::session sql;
  static bool OpenDb();
  static bool Execute(const std::string& _sql);
  static bool ExecuteTransaction(const std::string& _sql);
  static TableCreator Create();
  static Inserter Insert();
  template<typename T>
  static Selector<T> Select(const std::string& columns = "*");
};

// After Selector class definition, add the implementation:
template <typename T>
inline Selector<T> Database::Select(const std::string& columns) {
  return Selector<T>(columns);
}


