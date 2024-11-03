#pragma once
#include <functional>
#include <string>

class Database;
class TableCreator;

class ReparoCore {
public:
  void Render();

#ifdef DEBUG
public:
  static void RenderDebug();

private:  
  static void CreateDatabaseBtn();
  struct TableCreationInfo {
    std::string label;
    std::function<void()> create_function;
  };
  static std::vector<TableCreationInfo> table_config;
  static void InitializeTableBtns();
#endif // DEBUG

};