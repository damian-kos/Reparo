#include "modal.h"

ModalConfig& ModalConfig::Title(const std::string& _title) {
  is_on = true;
  title = _title;
  return *this;
}

ModalConfig& ModalConfig::Msg(const std::string& _msg) {
  message = _msg;
  return *this;
}

ModalConfig& ModalConfig::Callback(const ModalCallback& _callback) {
  callback = _callback;
  return *this;
}

ModalCallback BaseModal::Render() {
  auto callback = ModalCallback_None;
    // It is here so we can call ImGui's CloseCurrentPopup() from nested methodes in this one.
    // Otherwise Popup would open immediately after closing it.
    static bool _control_open = true;
    if (_control_open)
      ImGui::OpenPopup(config.title.c_str());
    if (ImGui::BeginPopupModal(config.title.c_str(), &config.is_on)) {
      callback = ModalContents() ? ModalCallback_Customer : callback;
      _control_open = false;
      ImGui::EndPopup();
    }
    else {
      _control_open = true;
    }
  return callback;
}

bool BaseModal::ModalContents() {
  bool action = false;
  if (ImGui::Button("Confirmmmm")) {
    action = true;
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return action;
}

bool const BaseModal::GetState() {
  return config.is_on;
}


bool CustomerModal::ModalContents() {
  bool action = false;
  customer.View();
  if (ImGui::Button("Confirm")) {
    action = true;
    customer.InsertToDb();
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return action;
}


bool RepairModal::ModalContents() {
  bool action = false;
  repair.View();
  if (ImGui::Button("Confirm")) {
    action = true;
    repair.InsertToDb();
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return action;
}

bool PartModal::ModalContents() { 
  bool action = false;
  part.View();
  if (ImGui::Button("Confirm")) {
    action = true;
    part.InsertToDb();
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return action;
}
