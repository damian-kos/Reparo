#include "modal.h"

ModalCallback BaseModal::Render() {
  auto callback = ModalCallback_None;
  ImGui::OpenPopup(data.title.c_str());
  if (ImGui::BeginPopupModal(data.title.c_str())) {
    ModalContents();
    ImGui::EndPopup();
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
  return data.is_on;
}

ModalCallback CustomerModal::Render() {
  auto callback = ModalCallback_None;
  // It is here so we can call ImGui's CloseCurrentPopup() from nested methodes in this one.
  // Otherwise Popup would open immediately after closing it.
  static bool _control_open = true;
  if (_control_open)
    ImGui::OpenPopup(data.title.c_str());
  if (ImGui::BeginPopupModal(data.title.c_str(), &data.is_on)) {
    callback = ModalContents() ? ModalCallback_Customer : callback;
    _control_open = false;
    ImGui::EndPopup();
  }
  else {
    _control_open = true;
  }
  return callback;
}

bool CustomerModal::ModalContents() {
  bool action = false;
  data.customer.View();
  if (ImGui::Button("Confirm")) {
    action = true;
    data.customer.InsertToDb();
    ImGui::CloseCurrentPopup();
  }
  if (ImGui::Button("Cancel")) {
    ImGui::CloseCurrentPopup();
  }
  return action;
}

bool const CustomerModal::GetState() {
  return data.is_on;
}
