#include "customer.h"
#include <iostream>
#include "imgui.h"
#include "../../src/RoLocalization.h"
#include "../../src/tables.h"
#include "../../src/database.h"
#include "../../src/modal.h"

Customer::Customer() {}

Customer& Customer::GetCustomer() {
#ifdef DEBUG
  std::cout << "Customer: " << id << " / " << phone << " / " << name << " / " << surname << " / " << email << std::endl;
#endif // DEBUG
  return *this;
}

void Customer::View() {
  if (id > 0) {
    ImGui::Text(_("Customer ID: %d"), id);
  }
  ImGui::Text(_("Phone: %s"), phone.c_str());
  ImGui::Text(_("Name: %s"), name.c_str());
  ImGui::Text(_("Surname: %s"), surname.c_str());
  ImGui::Text(_("Email: %s"), email.c_str());
  RoTable::Addresses(billing_addresses.Get().Lines(), ship_addresses.Get().Lines());
}

const std::string Customer::ToString() const {
  return std::to_string(id) + " | " + phone + " | " + name + " | " + surname + " | " + email;
}

void Customer::InsertModal() {
  ModalConfig config;
  config.Title(_("Insert Customer?"))
    .Msg(_("Are you sure?"));

  CustomerModal modal(*this, config);
  ModalManager::SetModal(modal);
}

void Customer::InsertToDb() {
  Database::Insert().Customer_(*this);
}
