#pragma once
#include <string>
#include <vector>

class TextField;

class Address {
public:

  class Data {
  public:
    Data(const Address& address);
    const int& ID() const;
    const std::vector<std::string> Lines() const;
    
  private:
    const Address& data;
  };

  Data Get() const { return Data(*this); }
  Address& SetID(int& _id);
  Address& SetLines(std::vector<TextField>& _address);

private:
  int id;
  std::vector<std::string> address;
};

class Customer{
public:
  Customer();
  Customer& GetCustomer();
  void View();
  const std::string ToString() const;
  void InsertToDb() const;

  int id = -1;
  std::string phone;
  std::string name;
  std::string surname;
  std::string email;
  Address billing_addresses;
  Address ship_addresses;
  static constexpr std::string_view table = "customers";
};