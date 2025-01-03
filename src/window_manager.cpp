#include "window_manager.h"

std::map<std::string, std::unique_ptr<RoWindow>> WindowFactory::windows;

void WindowFactory::AddWindow(const std::string& _window) {
  // Check if a window of the requested type is already open
  auto it = windows.find(_window);
  if (it != windows.end() && it->second->open) {
    return;
  }

  if (_window == "customer") {
    windows[_window] = std::make_unique<CustomerWin>();
  }
  else if (_window == "repair") {
    windows[_window] = std::make_unique<RepairWin>();
  }
  else if (_window == "device") {
    windows[_window] = std::make_unique<DeviceWin>();
  }
  else if (_window == "custom_device") {
    windows[_window] = std::make_unique<CustomDeviceWin>();
  }
  else if (_window == "parts") {
    windows[_window] = std::make_unique<PartsWin>();
  }
  else if (_window == "customers_view") {
    windows[_window] = std::make_unique<CustomerView>();
  }
  else if (_window == "repairs_view") {
    windows[_window] = std::make_unique<RepairView>();
  }
  else if (_window == "devices_view") {
    windows[_window] = std::make_unique<DevicesView>();
  }
  else if (_window == "parts_view") {
    windows[_window] = std::make_unique<InventoryView>();
  }
}

void WindowFactory::Render() {
  for (auto it = windows.begin(); it != windows.end(); ) {
    if (!it->second->open) {
      it = windows.erase(it); // Remove the window from the map if it's not open
    }
    else {
      it->second->Render(); // Render the window if it's open
      ++it;
    }
  }
}

