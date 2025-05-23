#include "device.h"
#include "../../src/modal.h"
#include <iostream>

void Device::UpdateModal() {
  ModalConfig config;
  config.Title(_("Update Device?"))
    .Msg(_("Are you sure?"))
    .State(ModalState_UpdateWindow);
  DeviceModal modal(*this, config);
  ModalManager::SetModal(modal);
}

const std::string Device::ToString() const{
  return std::to_string(id) + " | " + name + " | " + brand.name + " | " + type.name;
}

void CustomDevice::EditModal() {
  if (ImGui::Button(_("Convert to device"))) {
    ModalConfig config;
    config.Title(_("Confirm conversion?"))
      .Msg(_("This will update all of your current repairs."));
    CustomDeviceModal modal(*this, config);
    StackModal::SetModal(modal);
  }
}

void CustomDevice::DeleteModal() {
}
