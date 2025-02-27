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
  bool Equals(const Address& _other, bool _skip_id = false) const;
  std::string ToString(const std::string _delimiter, const std::string _align = "left") const;
  void Clear();

  bool operator==(const Address& _other) {
    Equals(_other, false);
  }

private:
  int id;
  std::vector<std::string> address;
};