#pragma once
#include "customer.h"
#include "simple_models.h"
#include "device.h"
#include "../../src/invoices.h"
#include "../../src/imgui_ex.h"

class Repair  {
public:
  int id;
  Customer customer;
  Device device;
  Category category;
  Color color;
  std::string sn_imei;
  std::string vis_note;
  std::string hid_note;
  double price;
  RepairState repair_state;
  int cust_device_id;
  ItemsContainer<RepairItem> items;
  std::tm created_at; // date
  std::tm updated_at; // date
  std::tm finished_at; // date
  static constexpr std::string_view table = "repairs";
  const std::string ToString() const;
  static void RenderRepairItemsTable(ItemsContainer<RepairItem>& _items, const bool _removable = false, const ItemsContainer<RepairItem>& _second = {});
  void View();
  void View(const Repair& _previous) ;
  void InsertModal();
  void InsertToDb();
  void UpdateModal();

  bool operator==(const Repair& other) const {
     //First, check if IDs are the same
    if (id != other.id) {
      return false;
    }

    // If IDs are the same, compare other attributes
    bool _are_equal = (customer == other.customer &&
      device == other.device &&
      category == other.category &&
      color == other.color &&
      sn_imei == other.sn_imei &&
      vis_note == other.vis_note &&
      hid_note == other.hid_note &&
      price == other.price &&
      repair_state == other.repair_state);

  //  // Compare the size of the items containers
    if (items.records.size() != other.items.records.size()) {
      return false;
    }

    for (const auto& item : items.records) {
      bool found = false;
      for (const auto& other_item : other.items.records) {
        if (item == other_item) {
          found = true;
          break;
        }
      }
      if (!found) {
        return false;
      }
    }
    return _are_equal;
  }

private:
  template <typename T>
  static void RenderItemsTableRow(const T& _current, const T& _previous, const char* format, bool _compare);

};

template<typename T>
inline void Repair::RenderItemsTableRow(const T& _current, const T& _previous, const char* format, bool _compare) {
  ImGui::TableNextColumn();
  if (_compare) {
    if constexpr (std::is_same_v<T, std::string>) 
      ImGui::TextWrappedColor(_current != _previous, "%s", _current.c_str());
    else 
      ImGui::TextWrappedColor(_current != _previous, format, _current);
  }
  else {
    if constexpr (std::is_same_v<T, std::string>)
      ImGui::TextWrapped("%s", _current.c_str());
    
    else 
      ImGui::TextWrapped(format, _current);
  }
}
