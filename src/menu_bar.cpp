#include "menu_bar.h"
#include "imgui.h"
#include "RoLocalization.h"
#include "window_manager.h"

void RoMenuBar::Render() {
  if (!ImGui::BeginMainMenuBar()) {
    return;
  }
  // New
  if (ImGui::BeginMenu(_("New"))) {
    Customer();
    Repair();
    Device();
    DeviceFromCustom();
    Parts();
    
    ImGui::EndMenu();
  }
  // Views
  if (ImGui::BeginMenu(_("Views"))) {
    CustomersView();
    RepairsView();
    DevicesView();
    InventoryView();
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

void RoMenuBar::DeviceFromCustom() {
  if (ImGui::MenuItem(_("Device from Custom Devices"))) {
    WindowFactory::AddWindow("custom_device");
  }
}

void RoMenuBar::Customer() {
  if (ImGui::BeginMenu(_("Customer"))) {
    if (ImGui::MenuItem(_("New Customer"))) {
      WindowFactory::AddWindow("customer");
    }
    ImGui::SeparatorText(_("Options"));
    if (ImGui::MenuItem(_("Edit Customer"))) {
    }
    ImGui::EndMenu();
  }
}

void RoMenuBar::Device() {
  if (ImGui::MenuItem(_("Device"))) {
    WindowFactory::AddWindow("device");
  }
}

void RoMenuBar::Repair() {
  if (ImGui::BeginMenu(_("Repair"))) {
    if (ImGui::MenuItem(_("New Repair"))) {
      WindowFactory::AddWindow("repair");
    }
    ImGui::SeparatorText(_("Options"));
    if (ImGui::MenuItem(_("Always open"))) {
    }
    ImGui::EndMenu();
  }
}

void RoMenuBar::Parts() {
  if (ImGui::BeginMenu(_("Items"))) {
    if (ImGui::MenuItem(_("New Item"))) {
      WindowFactory::AddWindow("parts");
    }
    ImGui::SeparatorText(_("Options"));
    if (ImGui::MenuItem(_("Always open"))) {
    }
    ImGui::EndMenu();
  }
}

void RoMenuBar::CustomersView() {
  if (ImGui::MenuItem(_("Customers"))) {
    WindowFactory::AddWindow("customers_view");
  }
}

void RoMenuBar::RepairsView() {
  if (ImGui::MenuItem(_("Repairs"))) {
    WindowFactory::AddWindow("repairs_view");
  }
}

void RoMenuBar::DevicesView() {
  if (ImGui::MenuItem(_("Devices"))) {
    WindowFactory::AddWindow("devices_view");
  }
}

void RoMenuBar::InventoryView() {
  if (ImGui::MenuItem(_("Items"))) {
    WindowFactory::AddWindow("parts_view");
  }
}
