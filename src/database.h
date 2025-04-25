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
  TableCreator& RepairUpdatesTable();
  TableCreator& TriggerContext();
};

class Inserter {
public:
  Inserter() = default;
  Inserter& Customer_(Customer& customer);
  Inserter& Device_(Device& device);
  Inserter& Repair_(Repair& repair);
  Inserter& Part_(Part& part);
  Inserter& Supplier_(Supplier& _supplier);
  Inserter& PurchaseInvoice_(PurchaseInvoice& _invoice);
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
  Updater& Repair_(Repair& repair);
  Updater& Customer_(Customer& _customer);
  Updater& Device_(Device& _device);
  Updater& Part_(Part& _part);

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

    Selector& From(const std::string& _table = "") {
      std::string table_name = _table;
      if (table_name.empty()) {
        if constexpr (std::is_class<T>::value && requires { T::table; }) {
          T temp;
          table_name = temp.table;
        }
      }
      sql << " FROM " << table_name;

      return *this;
    }

    Selector& Where(const std::string& _condition, const std::string _equal = "__DEFAULT__") {
      if (!_condition.empty()) {
        std::string _condition_formatted = "LOWER(" + _condition + ")";
        if (_equal == "__DEFAULT__") {
          sql << " WHERE " << _condition_formatted; // because _condition can be used with Like() method
        }
        else {
          std::string _equal_formatted = "LOWER('" + _equal + "')";
          sql << " WHERE " << _condition_formatted << " = " << _equal_formatted;
        }
      }
      return *this;
    }

    Selector& And(const std::string& _condition) {
      if(!_condition.empty())
        sql << " AND " << _condition;
      return *this;
    }

    Selector& AndLike(const std::string& column, const std::string& value) {
      if (!value.empty()) {
        sql << " AND LOWER(" << column << ") LIKE '%" << value << "%'";
      }
      return *this;
    }

    Selector& Like(const std::string& _like) {
        sql << " LIKE '%" << _like << "%'";
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

    Selector& Date(const std::string& _date) {
      sql << _date;
      return *this;
    }

    Selector& Last(const std::string& _primary_key = "id") {
      sql << " ORDER BY " << _primary_key << " DESC LIMIT 1";
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
  template <typename T>
  static Part Part_(const T& _value);
  template <typename T>
  static Supplier Supplier_(const T& _value);
  Address Address_(const int& _id, const std::string _type);
  template<typename T>
  static UpdateSet<T> GetItemsToUpdate(std::set<T>& _new_items, std::string _query, int _model_id);
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

template<typename T>
inline Part DBGet::Part_(const T& _value) {
  Database::OpenDb();
  std::string query;
  if (std::is_same_v<T, int>)
    query = "id = (:id) ";
  else
    query = "own_sku = (:own_sku)";

  Part part;
  T value = _value;
  Database::sql << "SELECT * FROM parts WHERE " << query,
    soci::use(value),
    soci::into(part);
  return part;
}

template<typename T>
inline Supplier DBGet::Supplier_(const T& _value) {
  Database::OpenDb();
  std::string query;
  if (std::is_same_v<T, int>)
    query = "id = (:id) ";
  else
    query = "name = (:name)";
  
  Supplier _supplier;
  T value = _value;
  Database::sql << "SELECT * FROM suppliers WHERE " << query,
    soci::use(value),
    soci::into(_supplier);
  
  return _supplier;
} 

// Retrieves a difference between items which we want to push to database and items which are already there.
// As a result it returns a pair of vectors: one with items to insert and one with items to delete.
// Works with Colors and Aliases -- so far.
template<typename T>
inline UpdateSet<T> DBGet::GetItemsToUpdate(std::set<T>& _new_items, std::string _query, int _model_id) {
    // Fetch existing items from database
    std::vector<T> existing_items;
    soci::rowset<T> rs = (Database::sql.prepare << _query, soci::use(_model_id));

    for (const auto& row : rs) {
      existing_items.push_back(row);
    }

    std::sort(existing_items.begin(), existing_items.end());

    std::vector<T> to_insert = Query::GetToInsert(_new_items, existing_items);
    std::vector<T> to_delete = Query::GetToDelete(_new_items, existing_items);

    return { to_insert, to_delete };
}
