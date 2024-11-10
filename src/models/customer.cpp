#include "customer.h"
#include <iostream>
#include "imgui.h"
#include "../../src/text_fields.h"
#include "../../src/RoLocalization.h"
#include "../../src/tables.h"

Customer::Customer() {}

Customer& Customer::GetCustomer() {
#ifdef DEBUG
  std::cout << "Customer: " << id << " / " << phone << " / " << name << " / " << surname << " / " << email << std::endl;
#endif // DEBUG
  return *this;
}

CustomerBuilder Customer::Set() {
  return CustomerBuilder(*this);
}

void Customer::View() {
  ImGui::Text(_("ID: %d"), id);
  ImGui::Text(_("Phone: %s"), phone.c_str());
  ImGui::Text(_("Name: %s"), name.c_str());
  ImGui::Text(_("Surname: %s"), surname.c_str());
  ImGui::Text(_("Email: %s"), email.c_str());
  RoTable::Addresses(billing_addresses, ship_addresses);
}

CustomerBuilder::CustomerBuilder(Customer& customer) : customer(customer) { }

CustomerBuilder& CustomerBuilder::ID(const int& id = -1) {
  customer.id = id;
  return *this;
}

CustomerBuilder& CustomerBuilder::Phone(const std::string& phone) {
  customer.phone = phone;
  return *this;
}

CustomerBuilder& CustomerBuilder::Name(const std::string& name) {
  customer.name = name;
  return *this;
}

CustomerBuilder& CustomerBuilder::Surname(const std::string& surname) {
  customer.surname = surname;
  return *this;
}

CustomerBuilder& CustomerBuilder::Email(const std::string& email) {
  customer.email = email;
  return *this;
}

CustomerBuilder& CustomerBuilder::BillingAddresses(const std::vector<TextField>& addresses) {
  for (int i = 0; i < addresses.size(); i++) {
    std::string line = addresses[i].Get();
    customer.billing_addresses.emplace_back(line);
  }
  return *this;
}

CustomerBuilder& CustomerBuilder::ShipAddresses(const std::vector<TextField>& addresses) {
  for (int i = 0; i < addresses.size(); i++) {
    std::string line = addresses[i].Get();
    customer.ship_addresses.emplace_back(line);
  }
  return *this;
}

Customer::Data::Data(const Customer& customer) : customer(customer) { }

int Customer::Data::ID() const {
  return customer.id;
}

const std::string& Customer::Data::Phone() const {
  return customer.phone;
}

const std::string& Customer::Data::Name() const {
  return customer.name;
}

const std::string& Customer::Data::Surname() const {
  return customer.surname;
}

const std::string& Customer::Data::Email() const {
  return customer.email;
}

const std::vector<std::string>& Customer::Data::BillingAddresses() {
  return customer.billing_addresses;
}

const std::vector<std::string>& Customer::Data::ShipAddresses() {
  return customer.ship_addresses;
}
