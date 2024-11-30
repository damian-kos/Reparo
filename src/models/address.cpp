#include "address.h"
#include "../../src/text_fields.h"

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