#pragma once
#include <string>
#include <vector>
#include "model_handler.h"

class Device {
public:
  Device();
  Device& GetDevice();
  template<typename T>
  Builder<T> Set();

  template<typename T>
  ModelData<T> Get() const;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;

  int id = -1;
  std::string name;
  std::string brand;
  std::string type;
  std::vector<std::string> colors;
  std::vector<std::string> aliases;
};

template<typename T>
inline Builder<T> Device::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Device::Get() const {
  return ModelData<T>(*this);
}