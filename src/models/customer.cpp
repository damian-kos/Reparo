#include "customer.h"
#include <iostream>
#include "imgui.h"

Customer::Customer() {}

Customer& Customer::Get() {
#ifdef DEBUG
  std::cout << "Customer: " << id << " / " << phone << " / " << name << " / " << surname << " / " << email << std::endl;
#endif // DEBUG
  return *this;
}

CustomerBuilder Customer::Set() {
  return CustomerBuilder(*this);
}

void Customer::View() {
  ImGui::Text("ID: %d", id);
  ImGui::Text("ID: %s", phone.c_str());
  ImGui::Text("ID: %s", name.c_str());
  ImGui::Text("ID: %s", surname.c_str());
  ImGui::Text("ID: %s", email.c_str());
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

CustomerBuilder& CustomerBuilder::BillingAddresses(const std::vector<std::string>& addresses) {
  customer.billing_addresses = addresses;
  return *this;
}

CustomerBuilder& CustomerBuilder::ShipAddresses(const std::vector<std::string>& addresses) {
  customer.ship_addresses = addresses;
  return *this;
}

