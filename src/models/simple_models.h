#pragma once
#include "model_handler.h"

class SimpleModel {
protected:
  int id = -1;
  std::string name;
};

class Brand : public SimpleModel {
public:
  Brand();
  Brand& GetBrand();

  template<typename T>
  Builder<T> Set();

  template<typename T>
  ModelData<T> Get() const;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;

};

template<typename T>
inline Builder<T> Brand::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Brand::Get() const {
  return ModelData<T>(*this);
}