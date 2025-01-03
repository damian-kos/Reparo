#pragma once
#include <functional>
#include <string>
#include "reparo_windows.h"
#include "modal.h"
#include "observer.h"
#include "views.h"
#include "menu_bar.h"
#include "window_manager.h"

class Database;
class TableCreator;

class ReparoCore : public IObserver {
public:
  ReparoCore();
  void Render();

private:
  RoMenuBar menu_bar;
  SimpleModelWin<Brand>    brand_win;
  SimpleModelWin<DeviceType> device_types_win;
  SimpleModelWin<Category> categories_win;
  SimpleModelWin<RepairState> repair_states_win;
  SimpleModelWin<PaymentMethod> payments_win;
  ModalManager modals;
  void Update() override;

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