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

ModalConfig& ModalConfig::State(const ModalState& _state) {
  state = _state;
  return *this;
}

ModalCallback BaseModal::Render() {
  auto callback = ModalCallback_None;
    // It is here so we can call ImGui's CloseCurrentPopup() from nested methodes in this one.
    // Otherwise Popup would open immediately after closing it.
    if (control_open)
      ImGui::OpenPopup(config.title.c_str(), ImGuiPopupFlags_NoReopen);
    if (ImGui::BeginPopupModal(config.title.c_str(), &config.is_on)) {
      callback = ModalContents() ? ModalCallback_Customer : callback;
      control_open = false;
      ImGui::EndPopup();
    }
    else {
      control_open = true;
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

  // Insert State
  if (config.state == ModalState_Insert || config.state == ModalState_Remove) {
    customer.View();
    if (ImGui::Button("Confirm")) {
      action = true;
      switch (config.state) {
      case ModalState_Insert:
        customer.InsertToDb();
        break;
      case ModalState_Remove:
        customer.RemoveFromDb();
        break;
      default:
        break;
      }
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      config.State(ModalState_None);
      ImGui::CloseCurrentPopup();
    }
  }

  UpdateWindow();
  UpdateView();
  return action;
}

bool CustomerModal::UpdateWindow() {
  if (config.state == ModalState_UpdateWindow) {
    customer_win.Render();
    ImGui::BeginDisabled(customer_win.GetError());
    if (ImGui::Button(_("Update"))) {
      ModalConfig _config;
      _config.Title(_("Confirm customer update?"))
        .State(ModalState_Update);
      CustomerModal _modal(customer_win.GetEntity(), customer_win.GetPrevious(), _config);
      StackModal::SetModal(_modal);
    }
    ImGui::EndDisabled();
  }
  return false;
}

bool CustomerModal::UpdateView() {
  if (config.state == ModalState_Update) {
    customer.View(previous);
    if (ImGui::Button("Confirm")) {
      // So we can update the customer which we selected in the table.
      customer.id = previous.id;
      customer.UpdateToDb();
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }
  return false;
}

bool RepairModal::ModalContents() {
  bool action = false;

  if (config.state == ModalState_Insert) {
    repair.View();
    if (ImGui::Button("Confirm")) {
      action = true;
      repair.InsertToDb();
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }

  if (config.state == ModalState_UpdateWindow) {
    repair_win.Render();
    if (ImGui::Button("Update")) {
      ModalConfig _config;
      _config.Title(_("Confirm update repair?"))
        .State(ModalState_Update);
      RepairModal _modal(repair_win.CreateRepair(), repair_win.GetPrevious(), _config);
      StackModal::SetModal(_modal);
    }
  }

  if (config.state == ModalState_Update) {
    repair.View(previous);
    if (ImGui::Button("Confirm")) {
      action = true;
      repair.UpdateString(previous);
      repair.UpdateToDb();
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }

  return action;
}

bool PartModal::ModalContents() {
  bool action = false;

  if (config.state == ModalState_Insert) {
    part.View();
    if (ImGui::Button("Confirm")) {
      action = true;
      part.InsertToDb();
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }

  if (config.state == ModalState_UpdateWindow) {
    part_win.Render();
    if (part_win.IsSubmitPressed()) {
      ModalConfig _config;
      _config.Title(_("Confirm update part?"))
        .State(ModalState_Update);
      PartModal _modal(part_win.CreatePart(), part_win.GetPrevious(), _config);
      StackModal::SetModal(_modal);
    }
  }

  if (config.state == ModalState_Update) {
    part.View();
    if (ImGui::Button("Confirm")) {
      action = true;
     /* repair.UpdateString(previous);
      repair.UpdateToDb();*/
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }
  }
  return action;
}

bool DeviceModal::ModalContents() {
  device_win.Render();
  return false;
}

bool CustomDeviceModal::ModalContents() {
  device_win.Render();
  return false;
}

