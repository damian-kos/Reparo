#pragma once
#include <string>
#include <vector>

class CustomerBuilder;

class Customer{
public:
  Customer();
  Customer& GetCustomer();
  CustomerBuilder Set();
  void View();

  class Data {
  public:
    Data(const Customer& customer);

    int ID() const;
    const std::string& Phone() const;
    const std::string& Name() const;
    const std::string& Surname() const;
    const std::string& Email() const;
    const std::vector<std::string>& BillingAddresses();
    const std::vector<std::string>& ShipAddresses();

  private:
    const Customer& customer;
  };

  Data Get() const { return Data(*this); }

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