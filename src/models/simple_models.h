#pragma once
#include "model_handler.h"

class SimpleModel {
protected:
  int id = -1;
  std::string name;
 static constexpr std::string_view table = "";
 static inline const std::string column = "";
};

class Brand : public SimpleModel {
public:
  Brand();
  ~Brand();
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
  static constexpr std::string_view table = "brands";
  static inline const std::string column = "brand";
};

template<typename T>
inline Builder<T> Brand::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> Brand::Get() const {
  return ModelData<T>(*this);
}