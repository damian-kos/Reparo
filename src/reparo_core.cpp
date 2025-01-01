  #include "reparo_core.h"
#include "imgui.h"
#include "database.h"

std::vector<ReparoCore::TableCreationInfo> ReparoCore::table_config;

ReparoCore::ReparoCore() {
  modals.Attach(this);
}

void ReparoCore::Render() {
  menu_bar.Render();
  WindowFactory::Render();
  brand_win.Render();
  device_types_win.Render();
  categories_win.Render();
  repair_states_win.Render();
  payments_win.Render();
  static bool show_device_win;
  ImGui::Checkbox("Device Win", &show_device_win);
  if(show_device_win)
    device_win.Render();
  repair_win.Render();
  parts_win.Render();
  custom_device_win.Render();
  if (modals.RenderModal() != ModalCallback_None) {
    modals.Notify();
  }

  static std::vector<Customer> customers = Database::Select<Customer>("c.*, COUNT(repairs.id) AS has_repairs")
    .From("customers c")
    .LeftJoin("repairs")
    .On("c.id = repairs.customer_id")
    .GroupBy("c.id")
    .All();

  static CustomerView customer_view(customers);
  customer_view.Render();

  static std::vector<Repair> repairs = Database::Select<Repair>("r.*, c.phone, c.name, rc.category, rs.state, ")
    .Coalesce(" (d.model, cd.model) AS model")
    .From("repairs r")
    .LeftJoin("customers c").On("c.id = r.customer_id")
    .LeftJoin("devices d").On("d.id = r.model_id")
    .LeftJoin("custom_devices cd").On("cd.id = r.cust_device_id")
    .LeftJoin("repair_categories rc").On("rc.id = r.category_id")
    .LeftJoin("repair_states rs").On("rs.id = r.repair_state_id")
    .OrderBy()
    .All();

  static RepairView repair_view(repairs);
  repair_view.Render();

  static InventoryView inventory_view({});
  inventory_view.Render();

  static DevicesView devices_view({});
  devices_view.Render();
  //ModalManager::RenderModal();
}

#ifdef DEBUG

void ReparoCore::Update() {
  brand_win.LoadData();
  device_types_win.LoadData();
  categories_win.LoadData();
  repair_states_win.LoadData();
  payments_win.LoadData();
}

void ReparoCore::RenderDebug() {
  InitializeTableBtns();

  ImGui::Begin("Debug");



    //List Creation Button for creating a tables in a database
    for (const auto& table : table_config) {
      if (ImGui::Button(table.label.c_str())) {
        table.create_function();
      }
    }
  ImGui::End();
}

void ReparoCore::CreateDatabaseBtn() {
  if (ImGui::Button("Create Database")) {
    //Database::CreateDatabase();
  }
}

void ReparoCore::InitializeTableBtns() {
  if (table_config.empty()) {  // Only initialize if empty
    table_config = {
      {"Open Database",                           []() {  Database::OpenDb(); }},
      {"Create All Tables",                       []() { Database::Create()
                                                            .BillingAddressesTable()
                                                            .ShipAddressesTable()
                                                            .CustomersTable()
                                                            .SuppliersTable()
                                                            .RepairStatesTable()
                                                            .ColorsTable()
                                                            .RepairCategoriesTable()
                                                            .DeviceTypesTable()
                                                            .BrandsTable()
                                                            .QualitiesTable()
                                                            .PaymentMethodsTable()
                                                            .InventoryActionsTable()
                                                            .DevicesTable()
                                                            .CustomDevicesTable()
                                                            .AliasesTable()
                                                            .ModelColorsTable()
                                                            .PartsTable()
                                                            .PurchaseInvoicesTable() 
                                                            .PurchaseInvoicesItemsTable()
                                                            .PartsHistoryTable()
                                                            .RepairsTable()
                                                            .RepairPartsTable()
                                                            .InvoicesTable()
                                                            .PartModelTable()
                                                            .PartModelAliasTable(); }},
      {"Create Billing Address Table",            []() { Database::Create().BillingAddressesTable(); }},
      {"Create Shipping Address Table",           []() { Database::Create().ShipAddressesTable(); }},
      {"Create Customer Table",                   []() { Database::Create().CustomersTable(); }},
      {"Create Supplier Table",                   []() { Database::Create().SuppliersTable(); }},
      {"Create Repair States Table",              []() { Database::Create().RepairStatesTable(); }},
      {"Create Colors Table",                     []() { Database::Create().ColorsTable(); }},
      {"Create Repair Categories Table",          []() { Database::Create().RepairCategoriesTable(); }},
      {"Create Device Types Table",               []() { Database::Create().DeviceTypesTable(); }},
      {"Create Brands Table",                     []() { Database::Create().BrandsTable(); }},
      {"Create Qualities Table",                  []() { Database::Create().QualitiesTable(); }},
      {"Create Payment Methods Table",            []() { Database::Create().PaymentMethodsTable(); }},
      {"Create Inventory Actions Table",          []() { Database::Create().InventoryActionsTable(); }},
      {"Create Devices Table",                    []() { Database::Create().DevicesTable(); }},
      {"Create Custom Devices Table",             []() { Database::Create().CustomDevicesTable(); }},
      {"Create Aliases Table",                    []() { Database::Create().AliasesTable(); }},
      {"Create Model Colors Table",               []() { Database::Create().ModelColorsTable(); }},
      {"Create Parts Table",                      []() { Database::Create().PartsTable(); }},
      {"Create Purchase Invoices Table",          []() { Database::Create().PurchaseInvoicesTable(); }},
      {"Create Purchases Invoices Items Table",   []() { Database::Create().PurchaseInvoicesItemsTable(); }},
      {"Create Parts History Table",              []() { Database::Create().PartsHistoryTable(); }},
      {"Create Repairs Table",                    []() { Database::Create().RepairsTable(); }},
      {"Create Repairs Parts Table",              []() { Database::Create().RepairPartsTable(); }},
      {"Create Invoices Table",                   []() { Database::Create().InvoicesTable(); }},
      {"Create Part Model Table",                 []() { Database::Create().PartModelTable(); }},
      {"Create Part Model Alias Table",           []() { Database::Create().PartModelAliasTable(); }},

    };
  //  std::cout << "Initialized table buttons with " << table_config.size() << " entries." << std::endl;
  }
}



#endif // DEBUG
