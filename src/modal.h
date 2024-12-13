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

class TextField;

enum ModalEvent {
  ModalEvent_None,
  ModalEvent_Insert,
  ModalEvent_Update,
  ModalEvent_Remove,
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
  std::string title;
  std::string message;
  bool is_on = false;
private:
  ModalCallback callback = ModalCallback_None;
};

// Customer modal builder
template <typename T>
class BuildSingleModal {
public:
  BuildSingleModal& SetData(this BuildSingleModal& self, const T& _model) {
    self.model = _model;
  }
private:
  T model;
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
};

class RepairModal : public BaseModal {
public:
  RepairModal(const Repair& _repair, ModalConfig& _config)
    : BaseModal(_config), repair(_repair) {
  }

  bool ModalContents() override;

private:
  Repair repair;
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
  CustomerModal(const Customer& _customer, ModalConfig& _config, ModalEvent _event)
    : BaseModal(_config), customer(_customer), event(_event) {
  }

  bool ModalContents() override;

private:
  Customer customer;
  ModalEvent event = ModalEvent_None;
};

class PartModal : public BaseModal {
 public:
    PartModal(const Part& _part, ModalConfig& _config)
    : BaseModal(_config), part(_part)  {}

     bool ModalContents() override;

 private:
  Part part;
};

class CustomDeviceModal : public BaseModal {
public:
  CustomDeviceModal(const CustomDevice& _device, ModalConfig& _config);

  ModalCallback Render() override;
  bool ModalContents() override;

private:
  CustomDevice device;
  DeviceWin device_win;
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
