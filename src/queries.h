#pragma once
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
class Part;
class Customer;
class Repair;
class Supplier;
class PurchaseInvoice;
struct InvoiceItem;
struct RepairItem;
struct Device;
struct Color;
struct Alias;


// For Device and its aliases and colors
template<typename T>
struct UpdateSet {
  std::vector<T> to_insert;
  std::vector<T> to_delete;
};

/// <summary>
/// Used to allow to run multiple queries in one Transaction without need to reopening database.
/// </summary>
namespace Query {
  int InsertBillingAddress(Customer& customer);
  int UpdateBillingAddress(Customer& customer);
  int InsertShippingAddress(Customer& customer);
  int UpdateShippingAddress(Customer& customer);
  int InsertCustomer(Customer& customer);
  int UpdateCustomer(Customer& customer);
  int InsertItem(Part& _part);
  int UpdateItem(Part& _part);
  int UpdateItem(InvoiceItem& _item);
  int UpdateRepair(Repair& repair);
  int UpdateRepairPart(RepairItem& _item);
  void InsertItemDevices(Part& _part);
  void InsertItemAliases(Part& _part);
  void UpdateItemDevices(Part& _part);
  void UpdateItemAliases(Part& _part);
  int InsertRepair(Repair& repair);
  int InsertCustomDevice(Repair& repair);
  void InsertRepairPart(RepairItem& _item);
  int InsertSupplier(Supplier& _supplier);
  int InsertPurchaseInvoice(PurchaseInvoice& _invoice);
  int InsertInvoiceItem(InvoiceItem& _item);
  void UpdateColors(UpdateSet<Color>& _colors, int _model_id);
  void UpdateAliases(UpdateSet<Alias>& _aliases, int _model_id);
  void UpdateDevice(Device& _device);
  template <typename T>
  int InsertSimpleModel(T& _model);
  void SetTriggerContext(const int& _context);
  void ResetTriggerContext();

  template <typename T>
  std::vector<T> GetToInsert(std::set<T>& _new_set, std::vector<T>& _existing_set) {
    std::vector<T> _to_insert;

    std::set_difference(_new_set.begin(), _new_set.end(),
      _existing_set.begin(), _existing_set.end(),
      std::back_inserter(_to_insert));

    return _to_insert;
  }

  template <typename T>
  std::vector<T> GetToDelete(std::set<T>& _new_set, std::vector<T>& _existing_set) {
    std::vector<T> _to_delete;

    std::set_difference(_existing_set.begin(), _existing_set.end(),
      _new_set.begin(), _new_set.end(),
      std::back_inserter(_to_delete));

    return _to_delete;
  }

}