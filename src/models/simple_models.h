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
  bool Edit();
  bool Delete();

};

// Derived structs
struct Brand : SimpleModel<Brand> {
  static constexpr std::string_view table = "brands";
  static inline const std::string column = "brand";
  static inline const std::string window_title = "Brands";
};

struct RepairState : SimpleModel<RepairState> {
  static constexpr std::string_view table = "repair_states";
  static inline const std::string column = "state";
  static inline const std::string window_title = "Repair States";
};

struct RepairCategory : SimpleModel<RepairCategory> {
  static constexpr std::string_view table = "repair_categories";
  static inline const std::string column = "category";
  static inline const std::string window_title = "Categories";
};

struct PaymentMethod : SimpleModel<PaymentMethod> {
  static constexpr std::string_view table = "payment_methods";
  static inline const std::string column = "method";
  static inline const std::string window_title = "Payment Methods";
};

struct Quality : SimpleModel<Quality> {
  static constexpr std::string_view table = "qualities";
  static inline const std::string column = "quality";
  static inline const std::string window_title = "Qualities";
};

struct DeviceType : SimpleModel<DeviceType> {
  static constexpr std::string_view table = "device_types";
  static inline const std::string column = "type";
  static inline const std::string window_title = "Device Types";
};

struct Color : SimpleModel<Color> {
  static constexpr std::string_view table = "colors";
  static inline const std::string column = "color";
  static inline const std::string window_title = "Colors";
};

template<typename Derived>
inline bool SimpleModel<Derived>::Delete() {
  bool action = false;
  if (ImGui::Button("Confirm")) {
    action = true;
    DeleteInDb();
    ImGui::CloseCurrentPopup();
  }
  return action;
}