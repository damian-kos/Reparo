#pragma once
#include "sqlite3.h"
#include <iostream>

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
  bool static OpenDb();
  static sqlite3* db;

public:
  static bool CreateDatabase();
};