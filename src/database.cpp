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
  char* err_msg = nullptr;
  if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &err_msg) == SQLITE_OK) {
    active = true;
    return true;
  }
  else {
    std::cerr << "Failed to start transaction: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
}

bool SQLTransaction::EnableForeignKeys() {
  char* err_msg = nullptr;
  if (sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, &err_msg) != SQLITE_OK) {
    std::cerr << "Failed to enable foreign key constraints: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}

bool SQLTransaction::Commit() {
  char* err_msg = nullptr;
  if (sqlite3_exec(db, "COMMIT;", NULL, NULL, &err_msg) == SQLITE_OK) {
    active = false;
    return true;
  }
  else {
    std::cerr << "Failed to Commit transaction: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
}

bool SQLTransaction::Rollback() {
  char* err_msg = nullptr;
  if (sqlite3_exec(db, "ROLLBACK;", NULL, NULL, &err_msg) == SQLITE_OK) {
    active = false;
    return true;
  }
  else {
    std::cerr << "Failed to Rollback transaction: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
}


sqlite3* Database::db = nullptr;

/// <summary>
/// If Database exists, opens it. Otherwise creates one, and opens it.
/// </summary>
/// <returns>Returns true if database could be open, false otherwise.  </returns>
bool Database::OpenDb() {
  int rc = sqlite3_open("resource/reparo.db", &db);
  if (rc != SQLITE_OK) {
     //std::cout << "DATABASE CAN'T BE OPEN" << std::endl;
    return false;
  }
   //std::cout << "DB IS OPEN" << std::endl;
  return true;
}

bool Database::CreateDatabase() {
  if (OpenDb()) { 
    //Log::msg("Database already exists");
    return false;  
  }
  return true;
}

