#include "reparo_windows.h"
#include "imgui.h"
#include "modal.h"
#include "models/customer.h"
#include "models/simple_models.h"
#include "database.h"
#include "models/device.h"

CustomerWin::CustomerWin()
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup)
  , name(_("Name"))
  , surname(_("Surname"))
  , email(_("Email"))
  , billing_address(5)
  , ship_address(5)
{ 
  for (int i = 0; i < 5; i++) {
    std::string label = _("Billing Addres Line ") + std::to_string(i + 1);
    TextField billing_field(label);
    billing_address[i] = billing_field;

    label = _("Shipping Addres Line ") + std::to_string(i + 1);
    TextField shipping_field(label);
    ship_address[i] = (shipping_field);
  }
}

void CustomerWin::Render() {
  static Modal<Customer> modal;

  ImGui::Begin(_("Insert Customer"), &open);
  phone.Render();
  name.Render();
  surname.Render();
  email.Render();
  Addresses();

  if (ImGui::Button(_("Submit Customer Details"))) {
    Customer customer;
    customer.Set<Customer>()
      .Phone(phone.Get())
      .Name(name.Get())
      .Surname(surname.Get())
      .Email(email.Get())
      .BillingAddresses(billing_address)
      .ShipAddresses(ship_address);
      
    Config<Customer> config{
      .title = _("Insert Customer?"),
      .msg = _("Are you sure?"),
      .caller = customer.GetCustomer(),
      .on_confirm = [customer]() { Database::Insert().Customer_(customer); }, // new
    };
    modal.SetConfig(config);
  }
  modal.Render();

  ImGui::End();
}

void CustomerWin::Addresses() {
  if (ImGui::CollapsingHeader("Addresses", ImGuiTreeNodeFlags_None)) {
    RoTable::AddressesInputs(billing_address, ship_address);
  }
}

DeviceWin::DeviceWin() 
  : name(_("Model"))
  , brand_combo(_("Choose brand"))
  , type_combo(_("Choose type"))
{
}

void DeviceWin::Render() { 
// It is suppposed to be a modal window so change ImGui::Begin() to BeginPopupModal later on.
  ImGui::Begin(_("Insert new device"), &open);

  static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerV |
    ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable;

  static std::vector<std::string> col_names = { _("Model"), _("Type"), _("Brand"), _("Colors"), _("Aliases") };

  if (ImGui::BeginTable("Devices", 5, flags)) {
    for (auto& name : col_names)
      ImGui::TableSetupColumn(name.c_str());
    ImGui::TableHeadersRow();
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    DeviceName();
 
    ImGui::TableNextColumn();
    type_combo.Render();

    ImGui::TableNextColumn();
    brand_combo.Render();

    ImGui::TableNextColumn();
    colors.Render();

    ImGui::TableNextColumn();
    aliases.Render();

    ImGui::EndTable();
  }

  if (ImGui::Button(_("Save"))) {
    Device device;
    auto build = device.Set<Device>();
    build.Name(name.Get())
      .Brand_(brand_combo.Get())
      .Type(type_combo.Get())
      .Colors(colors.Get())
      .Aliases(aliases.Get());
    Database::Insert().Device_(device);

  } 
  ImGui::End();
}

void DeviceWin::DeviceName() {
  static std::string model;
  std::string label = model.empty() ? _("Add model") : model;
  bool focus = false;
  focus = ImGui::Button(label.c_str());
  if (ImGui::BeginPopupContextItem("Name edit", ImGuiPopupFlags_MouseButtonLeft)) {
    if (focus) ImGui::SetKeyboardFocusHere();
    name.Render();
    if (ImGui::SmallButton(_("Save"))) {
      model = name.Get();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::SmallButton(_("Cancel"))) {
      name.Clear();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

