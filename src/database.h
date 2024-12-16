#pragma once
#include "../../src/soci_type_conversion_helpers.h"
#include <iostream>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <sstream>
#include <vector>
#include "queries.h"


class TableCreator;
class Inserter;
class Updater;
class Deleter;
template <typename T>
class Selector;
class DBGet;
class Customer;
class Database;

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
  static DBGet Get();
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
  Inserter& Customer_(Customer& customer);
  Inserter& Device_(Device& device);
  Inserter& Repair_(Repair& repair);
  Inserter& Part_(Part& part);
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
    throw; // Re-throw to allow higher-level error handling
  }
  catch (const soci::soci_error& e) {
    std::cerr << "General SOCI error in " << description << ": " << e.what() << std::endl;
    Database::sql.close();
    throw; // Re-throw to allow higher-level error handling
  }
  catch (const std::exception& e) {
    std::cerr << "Unexpected error in " << description << ": " << e.what() << std::endl;
    Database::sql.close();
    throw; // Re-throw to allow higher-level error handling
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
  template <typename T>
  Deleter& Customer_(const T& _value);

private:
  template<typename Func>
  Deleter& ExecuteTransaction(Func operation, const std::string& description);
};

template<typename T>
class Selector {
public:
    Selector(const std::string& _columns = "*") {
      std::string columns = _columns;
      if (columns.empty())
        columns = "*";
      sql << "SELECT " << columns;
    }

    Selector& From(const std::string& table = "") {
      std::string table_name = table;
      if constexpr (!std::is_same_v<T, std::string>) {
        if (table_name.empty()) {
          T temp;
          table_name = temp.table; 
        }
      }
      sql << " FROM " << table_name;
      
      return *this;
    }

    Selector& Where(const std::string& _condition) {
      if(!_condition.empty())
        sql << " WHERE " << _condition;
      return *this;
    }

    Selector& And(const std::string& _condition) {
      if(!_condition.empty())
        sql << " AND " << _condition;
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

    Selector& On(const std::string& _on) {
      sql << " ON " << _on;
      return *this;
    }

    Selector& GroupBy(const std::string& _group_by) {
      sql << " GROUP BY " << _group_by;
      return *this;
    }

    Selector& Coalesce(const std::string& _coalesce) {
      sql << " COALESCE" << _coalesce;
      return *this;
    }

    Selector& OrderBy(const std::string& _orderby = "", const int& _direction = 0) {
      if (_orderby.empty())
        return *this;
      std::string _direction_str = _direction == 1 ? " ASC" : " DESC";
      sql << " ORDER BY " << _orderby << _direction_str;
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

class DBGet {
public:
  template <typename T>
  static Customer Customer_(const T& _value);
  template <typename T>
  static Device Device_(const T& _value);
  template <typename T, typename SM>
  static SM SimpleModel_(const T& _value);
};

// After Selector class definition, add the implementation:
template <typename T>
inline Selector<T> Database::Select(const std::string& columns) {
  std::cout << typeid(T).name() << std::endl;
  return Selector<T>(columns);
}

template<typename T> 
inline Inserter& Inserter::OfSimpleModel(T& model) {
  return ExecuteTransaction(
    [&model]() {
      Query::InsertSimpleModel(model);
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

template<typename T>
inline Deleter& Deleter::Customer_(const T& _value) {
  return ExecuteTransaction(
    [&_value]() {

      std::string query;
      if (std::is_same_v<T, int>)
        query = "id = (:id) ";
      else
        query = "phone = (:phone)";

      Database::sql << "DELETE FROM customers WHERE " << query,
      soci::use(_value);

    },
    "Customer hass been removed with value " + std::to_string(_value ) + ""
  );
}

template<typename T, typename SM>
inline SM DBGet::SimpleModel_(const T& _value) {
  try {
    Database::OpenDb();

    // So we can use soci::into
    SM model;

    std::string query;
    if (std::is_same_v<T, int>)
      query = "id = (:id)";
    else
      query = model.column + " = (:" + model.column+ ")";
    // soci docs suggests this for safety
    T value = _value;
    Database::sql << "SELECT * FROM " << model.table << " WHERE " << query,
      soci::use(value),
      soci::into(model);

    Database::sql.close();

    return model;
  }
  catch (const soci::soci_error& e) {
    std::cerr << "[DB ERROR] Failed to execute query in SimpleModel_: "
      << e.what() << std::endl
      << "[Query Info] Table: " << SM{}.table
      << ", Value: " << _value << std::endl;
    throw; // Re-throw the exception to allow higher-level handling
  }
  catch (const std::exception& e) {
    // Log other standard exceptions
    std::cerr << "[ERROR] Unexpected error in SimpleModel_: "
      << e.what() << std::endl;
    throw; // Re-throw for higher-level handling
  }
}