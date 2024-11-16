#pragma once
#include <string>
#include <vector>
#include "model_handler.h"

class DeviceColor {
public:
  DeviceColor();
  DeviceColor(int id, int link_id);
  DeviceColor& GetDeviceColor();
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
  int link_id = -1;   // linked id in database in this case color_id
  static constexpr std::string_view table = "model_colors";
};

template<typename T>
inline Builder<T> DeviceColor::Set() {
  return Builder<T>(*this);
}

template<typename T>
inline ModelData<T> DeviceColor::Get() const {
  return ModelData<T>(*this);
}