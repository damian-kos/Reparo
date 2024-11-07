#pragma once
#include <string>
#include <vector>

class CustomerBuilder;

class Customer {
public:
  Customer();
  Customer Get();
  CustomerBuilder Set();

private:
  friend class CustomerBuilder;

  int id = -1;
  std::string phone;
  std::string name;
  std::string surname;
  std::string email;
  std::vector<std::string> billing_addresses;
  std::vector<std::string> ship_addresses;
};

class CustomerBuilder {
public:
  CustomerBuilder(Customer& customer);

  CustomerBuilder& ID(const int& id);
  CustomerBuilder& Phone(const std::string& phone);
  CustomerBuilder& Name(const std::string& name);
  CustomerBuilder& Surname(const std::string& surname);
  CustomerBuilder& Email(const std::string& phone);
  CustomerBuilder& BillingAddresses(const std::vector<std::string>& addresses);
  CustomerBuilder& ShipAddresses(const std::vector<std::string>& addresses);

private:
  Customer& customer;
};