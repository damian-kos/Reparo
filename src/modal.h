// modal.h
#pragma once
#include <string>
#include <imgui.h>
#include "models/customer.h"
#include <functional>
#include "RoLocalization.h"
#include "database.h"
#include <memory>
#include "observer.h"

enum ModalCallback {
  ModalCallback_None,
  ModalCallback_Customer,
  ModalCallback_SimpleModel,
};

class TextField;

// Define Modal
class ModalData {
public:
  std::string title;
  std::string message;
  bool is_on;
};

class CustomerModalData : public ModalData {
public:
  Customer customer;
};

template <typename T>
class SimpleModelModalData : public ModalData {
public:
  T model;
};

// Base modal builder using deducing this
template<typename Derived, typename Data>
class BuildModal {
public:
  // Use deducing this to return the most derived type
  Derived& Title(this Derived& self, const std::string& _title) {
    self.data.is_on = true;
    self.data.title = _title;
    return self;
  }

  Derived& Message(this Derived& self, const std::string& _msg) {
    self.data.message = _msg;
    return self;
  }

  Data Build(this const Derived& self) {
    return self.data;
  }

protected:
  Data data;
};

// Customer modal builder
class BuildCustomerModal : public BuildModal<BuildCustomerModal, CustomerModalData> {
public:
  BuildCustomerModal& SetCustomer(this BuildCustomerModal& self, const Customer& _customer) {
    self.data.customer = _customer;
    return self;
  }
};

/// <summary>
///  For objects which has only [int id] and [std::string name]. Brand, Quality etc.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class BuildSimpleModelModal : public BuildModal<BuildSimpleModelModal<T>, SimpleModelModalData<T>> {
public:
  BuildSimpleModelModal<T>& SetSimpleModel(this BuildSimpleModelModal& self, const T& _model) {
    self.data.model = _model; 
    return self;             
  }
};

class BaseModal {
public:
  BaseModal() = default;
  explicit BaseModal(const ModalData& _data) : data(_data) {}
  virtual ~BaseModal() = default;

  virtual ModalCallback Render();
  virtual bool ModalContents();

  virtual bool const GetState();
private:
  ModalData data;
};

class CustomerModal : public BaseModal {
public:
  explicit CustomerModal(const CustomerModalData& _data) : data(_data) {}

  ModalCallback Render() override;
  bool ModalContents() override;

  virtual bool const GetState();
private:
  CustomerModalData data;
};

template <typename T>
class SimpleModelModal : public BaseModal {
public:
  explicit SimpleModelModal(const SimpleModelModalData<T>& _data) : data(_data) {}

  ModalCallback Render() override;
  bool ModalContents() override;

  virtual bool const GetState();
private:
  SimpleModelModalData<T> data;

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
inline ModalCallback SimpleModelModal<T>::Render() {
  ModalCallback _callback = ModalCallback_None;
  // It is here so we can call ImGui's CloseCurrentPopup() from nested methodes in this one.
  // Otherwise Popup would open immediately after closing it.
  static bool _control_open = true;
  if(_control_open)
    ImGui::OpenPopup(data.title.c_str());
  if (ImGui::BeginPopupModal(data.title.c_str(), &data.is_on)) {
    _callback = ModalContents() ? ModalCallback_SimpleModel : _callback;
    _control_open = false;
    ImGui::EndPopup();
  }
  else {
    _control_open = true;
  }
  return _callback;
}

template<typename T>
inline bool SimpleModelModal<T>::ModalContents() {
  bool _action = false;
  ImGui::Text("%s", data.message.c_str());
  ImGui::Text("%d |", data.model.id);
  ImGui::SameLine();
  ImGui::Text("%s", data.model.name.c_str());
  if (data.title == _("Edit this record?")) {
    _action = data.model.Edit();
  }
  if (data.title == _("Delete this record?")) {
    _action = data.model.Delete();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return _action;
}

template<typename T>
inline bool const SimpleModelModal<T>::GetState() {
  return data.is_on;
}
