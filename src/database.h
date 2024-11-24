#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>
#include <vector>
#include <sstream>
#include "../../src/soci_type_conversion_helpers.h"
#include "models/simple_models.h"

class TableCreator;
class Inserter;
class Customer;
class Brand;
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
  Inserter& Device_(Device& device);
  template <typename T>
  Inserter& OfSimpleModel(T& model);

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

class Updater {
public:
  Updater() = default;
  template <typename T>
  Updater& OfSimpleModel(T& model);

private:
  template<typename Func>
  Updater& ExecuteTransaction(Func operation, const std::string& description);
};

class Deleter {
public:
  Deleter() = default;
  template <typename T>
  Deleter& OfSimpleModel(T& model);

private:
  template<typename Func>
  Deleter& ExecuteTransaction(Func operation, const std::string& description);
};

template<typename T>
class Selector {
public:
    Selector(const std::string& columns = "*") {
        sql << "SELECT " << columns;
    }

    Selector& From(const std::string& table = "") {
      std::string table_name = table;
      if (table_name.empty()) {
        T temp;
        table_name = temp.table;  // Assuming `T` has a member `table`.
      }
      sql << " FROM " << table_name;
      return *this;
    }

    Selector& Where(const std::string& condition) {
        sql << " WHERE " << condition;
        return *this;
    }

    Selector& Like(const std::string& like) {
      std::string clause =  "'%" + like + "%'";
      sql << " LIKE " << clause;
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

    Selector& InnerJoin(const std::string& join) {
      sql << " INNER JOIN " << join;
      return *this;
    }

    Selector& LeftJoin(const std::string& join) {
      sql << " LEFT JOIN " << join;
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
        }
        catch (const soci::soci_error& e) {
            std::cerr << "SOCI error executing query: " << e.what() << std::endl;
        }
        Database::sql.close(); 
        return results;
    }

    T One() {
        T result;
        try {
          Database::OpenDb();
          soci::statement st = (Database::sql.prepare << sql.str(), soci::into(result));
          st.execute();
          st.fetch();
        }
        catch (const soci::soci_error& e) {
          std::cerr << "SOCI error executing query: " << e.what() << std::endl;
        }
        Database::sql.close();
        return result;
    }

    std::string GetSql() const {
        return sql.str();
    }

private:
    std::ostringstream sql;
};

class Database {
public:
  Database() { OpenDb(); };
  static soci::session sql;
  static bool OpenDb();
  static bool Execute(const std::string& _sql);
  static bool ExecuteTransaction(const std::string& _sql);
  static TableCreator Create();
  static Inserter Insert();
  static Updater Update();
  static Deleter Delete();
  template<typename T>
  static Selector<T> Select(const std::string& columns = "*");
  static inline bool is_initialized = false;
};

// After Selector class definition, add the implementation:
template <typename T>
inline Selector<T> Database::Select(const std::string& columns) {
  return Selector<T>(columns);
}

template<typename T> 
inline Inserter& Inserter::OfSimpleModel(T& model) {
  return ExecuteTransaction(
    [&model]() {
      int id = 0;
      std::string column = " (" + model.column + ") ";
      std::string value = "(:" + model.column + ")";
      Database::sql << "INSERT INTO " << model.table << column
      << "VALUES " << value << " RETURNING id",
      soci::use(model.name)
      , soci::into(id);

      // Set the model's ID if needed
      model.id = id;
    },
      "SimpleModel insertion (Simple Model Name: " + model.name + " Type: " + typeid(T).name() + ")"
    );
}

template<typename T>
inline Updater& Updater::OfSimpleModel(T& model) {
  return ExecuteTransaction(
    [&model]() {
      std::string new_name = model.name;
      Database::sql << "UPDATE " << model.table << " SET " << model.column << "= :" << model.column << " WHERE id = :id",
        soci::use(new_name),
        soci::use(model.id);

    },
    "SimpleModel update (Simple Model Name: " + model.name + " Type: " + typeid(T).name() + ")"
  );
}

template<typename Func>
inline Updater& Updater::ExecuteTransaction(Func operation, const std::string& description){
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

template<typename Func>
inline Deleter& Deleter::ExecuteTransaction(Func operation, const std::string& description) {
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
inline Deleter& Deleter::OfSimpleModel(T& model) {
  return ExecuteTransaction(
    [&model]() {
      Database::sql << "DELETE FROM " << model.table << " WHERE id = :id",
        soci::use(model.id);
    },
    "SimpleModel update (Simple Model Name: " + model.name + " Type: " + typeid(T).name() + ")"
  );
}

