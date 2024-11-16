#pragma once
#include <string>
#include <vector>
#include "model_handler.h"
#include "simple_models.h"

class Alias {
public:
  Alias();
  Alias(int id, std::string name, int link_id = 0);
  Alias& GetAlias();
  template<typename T>
  Builder<T> Set();

  template<typename T>
  ModelData<T> Get() const;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;

  int id = -1;        // main id in database
  std::string name;
  int link_id = -1;   // linked id in database in this case device
  static constexpr std::string_view table = "aliases";
};

template<typename T>
inline Builder<T> Alias::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Alias::Get() const {
  return ModelData<T>(*this);
}