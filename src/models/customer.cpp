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

void Customer::UpdateModal() {
  ModalConfig config;
  config.Title(_("Update Customer?"))
    .Msg(_("Are you sure?"))
    .State(ModalState_UpdateWindow);
  CustomerModal modal(*this, config);
  ModalManager::SetModal(modal);
}

void Customer::UpdateToDb() {
  Database::Update().Customer_(*this);
}

bool Customer::Equals(const Customer& _other, bool _skip_id) const {
  if (!_skip_id && id != _other.id) {
    return false;
  }

  // If IDs are the same, compare other attributes
  return (phone == _other.phone &&
    name == _other.name &&
    surname == _other.surname &&
    email == _other.email &&
    billing_addresses.Equals(_other.billing_addresses, _skip_id) &&
    ship_addresses.Equals(_other.ship_addresses, _skip_id));
}

void Customer::LoadAddress() {
  std::cout << "Address loading" << std::endl;
  billing_addresses =  Database::Get().Address_(id, "billing");
  ship_addresses = Database::Get().Address_(id, "ship");
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
