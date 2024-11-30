#pragma once
#include <vector>
#include <string>

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
  Address& SetLines(std::vector<std::string>& _address);


private:
  int id;
  std::vector<std::string> address;
};