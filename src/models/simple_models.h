#pragma once
#include "imgui.h"
#include <iostream>
#include <string>

class Database;
class ModalManager;

// Struct to hold ID and Name
struct SimpleModelData {
  int id;
  std::string name;
};

// Base struct using CRTP
template <typename Derived>
struct SimpleModel {
public:
  // Default constructor
  SimpleModel() = default;

  // Constructor that sets the ID
  explicit SimpleModel(int id) : id(id) {}

  // Constructor that sets both ID and name
  SimpleModel(int id, std::string name) : id(id), name(std::move(name)) {}

  int id = -1;
  std::string name;

  // Returns the data as a SimpleModelData struct
  SimpleModelData Get() const {
    return { id, name };
  }

  // Static accessors for common properties
  static std::string_view Table() { return Derived::table; }
  static const std::string& Column() { return Derived::column; }
  static const std::string& WindowTitle() { return Derived::window_title; }
  const std::string ToString() const;
  void UpdateInDb();
  void DeleteInDb();
  void EditModal();
  bool Edit();
  void DeleteModal();
  bool Delete();

  operator bool() const {
    return id > 0 && !name.empty();
  }

};

// Derived structs
struct Brand : SimpleModel<Brand> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "brands";
  static inline const std::string column = "brand";
  static inline const std::string window_title = "Brands";
};

struct RepairState : SimpleModel<RepairState> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "repair_states";
  static inline const std::string column = "state";
  static inline const std::string window_title = "Repair States";
};

struct Category : SimpleModel<Category> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "repair_categories";
  static inline const std::string column = "category";
  static inline const std::string window_title = "Categories";
};

struct PaymentMethod : SimpleModel<PaymentMethod> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "payment_methods";
  static inline const std::string column = "method";
  static inline const std::string window_title = "Payment Methods";
};

struct Quality : SimpleModel<Quality> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "qualities";
  static inline const std::string column = "quality";
  static inline const std::string window_title = "Qualities";
};

struct DeviceType : SimpleModel<DeviceType> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "device_types";
  static inline const std::string column = "type";
  static inline const std::string window_title = "Device Types";
};

struct Color : SimpleModel<Color> {
  using SimpleModel::SimpleModel;
  static constexpr std::string_view table = "colors";
  static inline const std::string column = "color";
  static inline const std::string window_title = "Colors";
};

struct Price {
  /// <summary>
  /// Including VAT to get ex. VAT use ExVat()
  /// </summary>
  double amount = 0;
  bool ex_vat = false;
  double vat_rate = 20;

  double ExVat();
};