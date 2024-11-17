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

void Customer::View() {
  ImGui::Text(_("ID: %d"), id);
  ImGui::Text(_("Phone: %s"), phone.c_str());
  ImGui::Text(_("Name: %s"), name.c_str());
  ImGui::Text(_("Surname: %s"), surname.c_str());
  ImGui::Text(_("Email: %s"), email.c_str());
  RoTable::Addresses(billing_addresses.Get().Lines(), ship_addresses.Get().Lines());
}

Address::Data::Data(const Address& address) : data(address) { }

const int& Address::Data::ID() const {
  return data.id;
}

const std::vector<std::string> Address::Data::Lines() const {
  return data.address;
}

Address& Address::SetID(int& _id) {
  id = id;
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
