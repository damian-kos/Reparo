#include "customer.h"
#include <iostream>
#include "imgui.h"
#include "../../src/RoLocalization.h"
#include "../../src/tables.h"
#include "../../src/database.h"
#include "../../src/modal.h"

Customer::Customer() {}

Customer& Customer::GetEntity() {
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
  if (ImGui::BeginTable("Addresses", 2)) {
    ImGui::TableNextColumn();
    RoTable::Addresses(billing_addresses.Get().Lines(), _("Billing Address"));
    ImGui::TableNextColumn();
    RoTable::Addresses(ship_addresses.Get().Lines(), _("Shipping Address"));
    ImGui::EndTable();
  }
}

void Customer::View(const Customer& _previous) {
  if (id > 0) {
    ImGui::Text(_("Customer ID: %d"), id);
  }
  ImGui::TextWrappedColor(phone != _previous.phone, _("Phone: %s"), phone.c_str());
  ImGui::TextWrappedColor(name != _previous.name, _("Name: %s"), name.c_str());
  ImGui::TextWrappedColor(surname != _previous.surname, _("Surname: %s"), surname.c_str());
  ImGui::TextWrappedColor(email != _previous.email, _("Email: %s"), email.c_str());
  if (ImGui::BeginTable("Addresses", 2)) {
    ImGui::TableNextColumn();
    RoTable::Addresses(billing_addresses.Get().Lines(), _("Billing Address"));
    ImGui::TableNextColumn();
    RoTable::Addresses(ship_addresses.Get().Lines(), _("Shipping Address"));
    ImGui::EndTable();
  }

}

const std::string Customer::ToString() const {
  return std::to_string(id) + " | " + phone + " | " + name + " | " + surname + " | " + email;
}

void Customer::InsertModal() {
  ModalConfig config;
  config.Title(_("Insert Customer?"))
    .Msg(_("Are you sure?"))
    .State(ModalState_Insert);

  CustomerModal modal(*this, config);
  ModalManager::SetModal(modal);
}

void Customer::InsertToDb() {
  Database::Insert().Customer_(*this);
}

void Customer::RemoveModal() const {
  ModalConfig config;
  config.Title(_("Remove Customer?"))
    .Msg(_("Are you sure?"))
    .State(ModalState_Remove);

  CustomerModal modal(*this, config);
  ModalManager::SetModal(modal);
}

void Customer::RemoveFromDb() {
  Database::Delete().Customer_(id);
}

void Customer::SetRepairs(const int& _count) {
  if (_count > 0) 
    has_repairs = true;
  else
    has_repairs = false;
}

bool Customer::HasRepairs() const {
  return has_repairs;
}
