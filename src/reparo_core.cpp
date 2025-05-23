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
  if (modals.RenderModal() != ModalCallback_None) {
    modals.Notify();
  }
}

#ifdef DEBUG

void ReparoCore::Update() {
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
                                                            .PartModelAliasTable()
                                                            .RepairUpdatesTable(); }},
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
      {"Create Repair Updates Table",             []() { Database::Create().RepairUpdatesTable(); }},
      {"Create Trigger Context Table",            []() { Database::Create().TriggerContext(); }},


    };
  //  std::cout << "Initialized table buttons with " << table_config.size() << " entries." << std::endl;
  }
}



#endif // DEBUG
