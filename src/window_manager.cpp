#include "window_manager.h"

std::map<std::string, std::unique_ptr<RoWindow>> WindowFactory::windows;

void WindowFactory::AddWindow(const std::string& _window) {
  // Check if a window of the requested type is already open
  auto it = windows.find(_window);
  if (it != windows.end() && it->second->open) {
    return;
  }

  // New windows
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
  else if (_window == "purchase_invoice") {
    windows[_window] = std::make_unique<PurchaseInvoiceWin>();
  }
  else if (_window == "supplier") {
    windows[_window] = std::make_unique<SupplierWin>();
  }
  // Views
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
  else if (_window == "categories") {
    windows[_window] = std::make_unique<SimpleModelWin<Category>>();
  }
  else if (_window == "brands") {
    windows[_window] = std::make_unique<SimpleModelWin<Brand>>();
  }
  else if (_window == "device_types") {
    windows[_window] = std::make_unique<SimpleModelWin<DeviceType>>();
  }
  else if (_window == "repair_states") {
    windows[_window] = std::make_unique<SimpleModelWin<RepairState>>();
  }
  else if (_window == "qualities") {
    windows[_window] = std::make_unique<SimpleModelWin<Quality>>();
  }
  else if (_window == "payment_methods") {
    windows[_window] = std::make_unique<SimpleModelWin<PaymentMethod>>();
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

