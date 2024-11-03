#pragma once
#include "reparo_core.h"
#include "imgui.h"
#include "database.h"
#include "RoLocalization.h"

std::vector<ReparoCore::TableCreationInfo> ReparoCore::table_config;

void ReparoCore::Render() {

}

#ifdef DEBUG

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
    Database::CreateDatabase();
  }
}

void ReparoCore::InitializeTableBtns() {
  if (table_config.empty()) {  // Only initialize if empty
    table_config = {
      {"Create Database",                 []() {  Database::CreateDatabase(); }},
      {"Create Billing Address Table",    []() { Database::Create().BillingAddressesTable(); }},
      {"Create Shipping Address Table",   []() { Database::Create().ShipAddressesTable(); }},
      {"Create Customer Table",           []() { Database::Create().CustomersTable(); }},
      {"Create Supplier Table",           []() { Database::Create().SuppliersTable(); }},
      {"Create Repair States Table",      []() { Database::Create().RepairStatesTable(); }},
      {"Create Colors Table",             []() { Database::Create().ColorsTable(); }},
      {"Create Repair Categories Table",  []() { Database::Create().RepairCategoriesTable(); }},
      {"Create Device Types Table",       []() { Database::Create().DeviceTypesTable(); }},
      {"Create Brands Table",             []() { Database::Create().BrandsTable(); }},
      {"Create Qualities Table",          []() { Database::Create().QualitiesTable(); }},
      {"Create Payment Methods Table",    []() { Database::Create().PaymentMethodsTable(); }},
      {"Create Inventory Actions Table",  []() { Database::Create().InventoryActionsTable(); }},
      {"Create Devices Table",            []() { Database::Create().DevicesTable(); }},
      {"Create Aliases Table",            []() { Database::Create().AliasesTable(); }},
      {"Create Model Colors Table",       []() { Database::Create().ModelColorsTable(); }},
    };
    std::cout << "Initialized table buttons with " << table_config.size() << " entries." << std::endl;
  }
}



#endif // DEBUG
