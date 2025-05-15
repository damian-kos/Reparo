// modal.h
#pragma once
#include <string>
#include <imgui.h>
#include "models/customer.h"
#include "models/parts.h"
#include "models/device.h"
#include "RoLocalization.h"
#include "database.h"
#include <memory>
#include "observer.h"
#include "reparo_windows.h"
#include "window_manager.h"

class TextField;

enum ModalState {
  ModalState_None,
  ModalState_Insert,
  ModalState_UpdateWindow,
  ModalState_Update,
  ModalState_Remove,
};

enum ModalCallback {
  ModalCallback_None,
  ModalCallback_Customer,
  ModalCallback_SimpleModel,
};

// Define Modal
struct ModalConfig {
public:
  ModalConfig& Title(const std::string& _title);
  ModalConfig& Msg(const std::string& _msg);
  ModalConfig& Callback(const ModalCallback& callback);
  ModalConfig& State(const ModalState& _state);
  std::string title;
  std::string message;
  bool is_on = false;
  ModalState state = ModalState_None;
private:
  ModalCallback callback = ModalCallback_None;
};

class BaseModal {
public:
  BaseModal() = default;
  explicit BaseModal(const ModalConfig& _config) : config(_config) {}
  virtual ~BaseModal() = default;

  virtual ModalCallback Render();
  virtual bool ModalContents();
  virtual bool const GetState();

protected:
  ModalConfig config;
  bool control_open = true;
};

class RepairModal : public BaseModal {
public:
  RepairModal(const Repair& _repair, ModalConfig& _config)
    : BaseModal(_config), repair(_repair) {
    repair_win = RepairWin(repair);
  }

  RepairModal(const Repair& _repair, const Repair& _previous, ModalConfig& _config)
    : BaseModal(_config), repair(_repair), previous(_previous) {
  }

  bool ModalContents() override;

private:
  Repair repair;

  // ModalState_UpdateWindow
  RepairWin repair_win;
  // ModalState_Update
  Repair previous;
};

template <typename T>
class SimpleModelModal : public BaseModal {
public:
  explicit SimpleModelModal(const T& _model, ModalConfig& _config)
    : BaseModal(_config), model(_model) {
  }

  //ModalCallback Render() override;
  bool ModalContents() override;

  //virtual bool const GetState();
private:
  T model;
};

class CustomerModal : public BaseModal {
public:
  CustomerModal(const Customer& _customer, ModalConfig& _config)
    : BaseModal(_config), customer(_customer) {
    customer.LoadAddress();
    customer_win = CustomerWin(customer);
  }

  CustomerModal(const Customer& _customer, const Customer& _previous, ModalConfig& _config)
    : BaseModal(_config), customer(_customer), previous(_previous) {
  }

  bool ModalContents() override;

private:
  Customer customer;

  // ModalState_UpdateWindow
  bool UpdateWindow();
  CustomerWin customer_win;
  Customer previous; 

  // ModalState_Update
  bool UpdateView();

};

class PartModal : public BaseModal {
 public:
    PartModal(const Part& _part, ModalConfig& _config)
    : BaseModal(_config), part(_part)  {
      part_win = PartsWin(part);
      view_data = part.CreateViewData(previous);
    }
    
    PartModal(const Part& _part, const Part& _previous, ModalConfig& _config)
      : BaseModal(_config), part(_part), previous(_previous) {  
      view_data = part.CreateViewData(previous);
    }

     bool ModalContents() override;

 private:
  Part part;
   
  // ModalState_UpdateWindow
  PartsWin part_win;
  // ModalState_Update
  Part previous;
  std::vector<ModelDataView> view_data;
};

class DeviceModal : public BaseModal {
public:
  DeviceModal(ModalConfig& _config)
    : BaseModal(_config) {
  }

  DeviceModal(const Device& _device, ModalConfig& _config)
    : BaseModal(_config), device_win(_device) {
  }

  bool ModalContents() override;

private:
  DeviceWin device_win;
};

class CustomDeviceModal : public BaseModal {
public:
  CustomDeviceModal(const CustomDevice& _device, ModalConfig& _config)
    :BaseModal(_config), device(_device), device_win(_device) {}

  bool ModalContents() override;

private:
  CustomDevice device;
  DeviceWin device_win;
};

// Disabled
class PurchaseInvoiceModal : public BaseModal {
public:
  PurchaseInvoiceModal(const PurchaseInvoice& _invoice, ModalConfig& _config)
    : BaseModal(_config), invoice(_invoice) {
    invoice_win = PurchaseInvoiceWin(invoice);
  }

  bool ModalContents() override;

private:
  PurchaseInvoice invoice;
  PurchaseInvoiceWin invoice_win;
};

class SupplierModal : public BaseModal {
public:
  SupplierModal(const Supplier& _supplier, ModalConfig& _config)
    : BaseModal(_config), supplier(_supplier) {
    supplier_win = SupplierWin(supplier);
  }

  SupplierModal(const Supplier& _supplier, const Supplier& _previous, ModalConfig& _config)
    : BaseModal(_config), supplier(_supplier), previous(_previous) {
    view_data = supplier.CreateViewData(previous);
  }

  bool ModalContents() override;

private:
  Supplier supplier;
  Supplier previous;
  SupplierWin supplier_win;
  std::vector<ModelDataView> view_data;

};

template <typename T>
class ConfirmCancelModal : public BaseModal {
public:
  ConfirmCancelModal(const T& _model, ModalConfig& _config)
    : BaseModal(_config), model(_model) {
  }

  bool ModalContents() override;
private:
  T model;
  
};

/// <summary>
/// ModalManager is running in reparo_core. If we want to use modal in modal use StackModalManager within Model's Render() methods.
/// </summary>
class ModalManager : public Subject {
public:
  // Use unique_ptr to avoid object slicing and manage memory
  template <typename T>
  static void SetModal(const T& modal) {
    // Use make_unique to create a dynamically allocated derived class object
    modal_win = std::make_unique<T>(modal);
  }

  ModalCallback RenderModal() {
    // Check if modal exists before rendering
    if (modal_win) {
      callback = modal_win->Render();
      if(!modal_win->GetState()) {
        ClearModal();
      }
    }
    return callback;
  }

  // Method to manually reset/clear the modal
  static void ClearModal() {
    std::cout << "Modal has been cleared" << std::endl;
    modal_win.reset(); // Explicitly deletes the current modal and sets to null
  }

  // Method to check if a modal is currently set
  static bool HasModal() {
    return modal_win != nullptr;
  }

  static void SetCallback(const ModalCallback _callback = ModalCallback_None) {
    callback = _callback;
  }

private:
  // Store the modal by value
  static inline ModalCallback callback = ModalCallback_None;
  static inline std::unique_ptr<BaseModal> modal_win;
};

class StackModal {
public:
  // Use unique_ptr to avoid object slicing and manage memory
  template <typename T>
  static void SetModal(const T& modal) {
    // Use make_unique to create a dynamically allocated derived class object
    modal_win = std::make_unique<T>(modal);
  }

  static void RenderModal() {
    // Check if modal exists before rendering
    auto _callback = ModalCallback_None;
    if (modal_win) {
      _callback = modal_win->Render();
      if (!modal_win->GetState()) {
        ClearModal();
      }
    }
    ModalManager::SetCallback(_callback);
  }

  static void ClearModal() {
    std::cout << "Stacked modal has been cleared" << std::endl;
    modal_win.reset(); // Explicitly deletes the current modal and sets to null
  }

private:
  static inline std::unique_ptr<BaseModal> modal_win;
};

template<typename T>
inline bool SimpleModelModal<T>::ModalContents() {
  bool _action = false;
  ImGui::Text("%s", config.message.c_str());
  ImGui::Text("%d |", model.id);
  ImGui::SameLine();
  ImGui::Text("%s", model.name.c_str());
  if (config.title == _("Edit this record?")) {
    _action = model.Edit();
  }
  if (config.title == _("Delete this record?")) {
    _action = model.Delete();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return _action;
}

template<typename T>
inline bool ConfirmCancelModal<T>::ModalContents() {
  bool action = false;

  ImGui::Text(config.message.c_str());
  if constexpr (requires { model.View(); }) {
    model.View();
  }

  if (ImGui::Button(BTN_CONFIRM)) {
    action = true;
    if (config.state == ModalState_Insert) {
      model.InsertToDb();
    }
    else if (config.state == ModalState_Update) {
      model.UpdateToDb();
    }
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button(BTN_CANCEL)) {
    ImGui::CloseCurrentPopup();
  }

  return action;
}
