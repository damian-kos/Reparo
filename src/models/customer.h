#pragma once
#include <string>
#include "address.h"



class Customer{
public:
  Customer();
  Customer& GetEntity();
  void View();
  void View(const Customer& _previous);
  const std::string ToString() const;
  void InsertModal();
  void InsertToDb();
  void RemoveModal() const;
  void RemoveFromDb();
  void UpdateModal();
  void UpdateToDb();

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

  operator bool() const {
    return id > 0;
  }

  void SetRepairs(const int& _count);
  bool HasRepairs() const;

private:
  bool has_repairs = false;
};
