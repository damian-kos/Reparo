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
  bool Equals(const Customer& _other, bool skip_id = false) const;
  void LoadAddress();

  int id = -1;
  std::string phone;
  std::string name;
  std::string surname;
  std::string email;
  Address billing_addresses;
  Address ship_addresses;
  static constexpr std::string_view table = "customers";

  bool operator==(const Customer& _other) const {
    return Equals(_other);
  }

  bool operator!=(const Customer& _other) const {
    return !(*this == _other);
  }

  operator bool() const {
    return id > 0;
  }

  void SetRepairs(const int& _count);
  bool HasRepairs() const;

private:
  bool has_repairs = false;
};
