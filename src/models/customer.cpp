#include "customer.h"
#include <iostream>
#include "imgui.h"
#include "../../src/text_fields.h"
#include "../../src/RoLocalization.h"
#include "../../src/tables.h"
#include "../../src/database.h"

Customer::Customer() {}

Customer& Customer::GetCustomer() {
#ifdef DEBUG
  std::cout << "Customer: " << id << " / " << phone << " / " << name << " / " << surname << " / " << email << std::endl;
#endif // DEBUG
  return *this;
}

void Customer::View() {
  ImGui::Text(_("ID: %d"), id);
  ImGui::Text(_("Phone: %s"), phone.c_str());
  ImGui::Text(_("Name: %s"), name.c_str());
  ImGui::Text(_("Surname: %s"), surname.c_str());
  ImGui::Text(_("Email: %s"), email.c_str());
  RoTable::Addresses(billing_addresses.Get().Lines(), ship_addresses.Get().Lines());
}

const std::string Customer::ToString() const {
  return std::to_string(id) + " | " + phone + " | " + name + " | " + surname + " | " + email;
}

void Customer::InsertToDb() {
  Database::Insert().Customer_(*this);
}

Address::Data::Data(const Address& address) : data(address) { }

const int& Address::Data::ID() const {
  return data.id;
}

const std::vector<std::string> Address::Data::Lines() const {
  return data.address;
}

Address& Address::SetID(int& _id) {
  id = _id;
  return *this;
}

Address& Address::SetLines(std::vector<TextField>& _address) {
  int i = 0;
  for (auto& field : _address) {
    address.push_back(field.Get());
    ++i;
  }
  return *this;
}

Address& Address::SetLines(std::vector<std::string>& _address) {
  int i = 0;
  for (auto& field : _address) {
    address.push_back(field);
    ++i;
  }
  return *this;
}