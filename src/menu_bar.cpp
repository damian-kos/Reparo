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
    StockReceived();
    SupplierRecord();
    ImGui::EndMenu();
  }
  // Views
  if (ImGui::BeginMenu(_("Views"))) {
    CustomersView();
    RepairsView();
    DevicesView();
    InventoryView();
    PurchaseInvoicesView();
    AttributesView();
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

void RoMenuBar::StockReceived() {
  if (ImGui::BeginMenu(_("Stock Received"))) {
    if (ImGui::MenuItem(_("From Purchase Invoice"))) {
      WindowFactory::AddWindow("purchase_invoice");
    }
    if (ImGui::MenuItem(_("Internal arrival"))) {
      WindowFactory::AddWindow("internal_arrival");
    }
    ImGui::SeparatorText(_("Options"));
    if (ImGui::MenuItem(_("Always open"))) {
    }
    ImGui::EndMenu();
  }
}

void RoMenuBar::SupplierRecord() {
  if (ImGui::MenuItem(_("Supplier"))) {
    WindowFactory::AddWindow("supplier");
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

void RoMenuBar::PurchaseInvoicesView() {
  if (ImGui::MenuItem(_("Purchase Invoices"))) {
    WindowFactory::AddWindow("purchase_invoices_view");
  }
}

void RoMenuBar::AttributesView() {
  if (ImGui::BeginMenu(_("Attributes"))) {
    if (ImGui::MenuItem(_("Repair Categories"))) {
      WindowFactory::AddWindow("categories");
    }
    if (ImGui::MenuItem(_("Brands"))) {
      WindowFactory::AddWindow("brands");
    }
    if (ImGui::MenuItem(_("Device Types"))) {
      WindowFactory::AddWindow("device_types");
    }
    if (ImGui::MenuItem(_("Repair States"))) {
      WindowFactory::AddWindow("repair_states");
    }
    if (ImGui::MenuItem(_("Qualities"))) {
      WindowFactory::AddWindow("qualities");
    }
    if (ImGui::MenuItem(_("Payment Methods"))) {
      WindowFactory::AddWindow("payment_methods");
    }
    ImGui::EndMenu();
  }
}