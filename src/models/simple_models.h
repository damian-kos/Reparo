#pragma once
#include "model_handler.h" 
#include <iostream>

class SimpleModel {
public:
  SimpleModel() = default; // Default constructor
  SimpleModel(int id, std::string name) : id(id), name(std::move(name)) {} // Parameterized constructor
protected:
 int id = -1;
 std::string name;
};

template <typename Derived>
class ModelBase : public SimpleModel {
public:
  ModelBase() = default;
  ModelBase(int id, std::string name);
  ~ModelBase() = default;

  Derived& GetModel() {
    //std::cout << id << " " << name << std::endl;
    return static_cast<Derived&>(*this);
  }

  const Derived& GetModel() const {
    return static_cast<const Derived&>(*this);
  }

  template <typename T>
  Builder<T> Set() {
    return Builder<T>(GetModel());  // Directly pass GetModel(), which is already a reference to Derived
  }

  template <typename T>
  ModelData<T> Get() const {
    return ModelData<T>(GetModel());  // Directly pass *this as const reference to Derived
  }
};

class Brand : public ModelBase<Brand> {
public:
  Brand()  = default;
  Brand(int id, std::string name) : ModelBase<Brand>(id, std::move(name)) {}
  ~Brand() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "brands";
  static inline const std::string column = "brand";
  static inline const std::string window_title = "Brands";
};

class RepairState : public ModelBase<RepairState> {
public:
  RepairState()  = default;
  ~RepairState() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "repair_states";
  static inline const std::string column = "state";
  static inline const std::string window_title = "Repair States";
};

class RepairCategory : public ModelBase<RepairCategory> {
public:
  RepairCategory()  = default;
  ~RepairCategory() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "repair_categories";
  static inline const std::string column = "category";
  static inline const std::string window_title = "Categories";
};

class PaymentMethod : public ModelBase<PaymentMethod> {
public:
  PaymentMethod() = default;
  ~PaymentMethod() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "payment_methods";
  static inline const std::string column = "method";
  static inline const std::string window_title = "Payment methods";
};

class Quality : public ModelBase<Quality> {
public:
  Quality() = default;
  ~Quality() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "qualities";
  static inline const std::string column = "quality";
  static inline const std::string window_title = "Qualities";
};

class DeviceType : public ModelBase<DeviceType> {
public:
  DeviceType() = default;
  DeviceType(int id, std::string name) : ModelBase<DeviceType>(id, std::move(name)) {}
  ~DeviceType() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "device_types";
  static inline const std::string column = "type";
  static inline const std::string window_title = "Device types";
};

class Color : public ModelBase<Color> {
public:
  Color() = default;
  Color(int id, std::string name) : ModelBase<Color>(id, std::move(name)) {}
  ~Color() = default;

private:
  template<typename T>
  friend class Builder;
  template<typename T>
  friend class ModelData;
  static constexpr std::string_view table = "colors";
  static inline const std::string column = "color";
  static inline const std::string window_title = "Colors";
};

template<typename Derived>
inline ModelBase<Derived>::ModelBase(int id, std::string name)
  : SimpleModel(id, std::move(name)) { } // Delegates initialization to SimpleModel
