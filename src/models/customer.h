#pragma once
#include <string>
#include "address.h"


class Customer{
public:
  Customer();
  Customer& GetCustomer();
  void View();
  const std::string ToString() const;
  void InsertToDb();

  int id = -1;
  std::string phone;
  std::string name;
  std::string surname;
  std::string email;
  Address billing_addresses;
  Address ship_addresses;
  static constexpr std::string_view table = "customers";

  bool operator==(const Customer& other) const {
    // First, check if IDs are the same
    if (id != other.id) {
      return false;
    }

    // If IDs are the same, compare other attributes
    return (phone == other.phone &&
      name == other.name &&
      surname == other.surname &&
      email == other.email &&
      billing_addresses.Get().ID() == other.billing_addresses.Get().ID() &&
      ship_addresses.Get().ID() == other.ship_addresses.Get().ID());
  }

  bool operator!=(const Customer& other) const {
    return !(*this == other);
  }
};
