#include "reparo_windows.h"
#include "imgui.h"
#include "modal.h"
#include "models/customer.h"
#include "models/simple_models.h"
#include "database.h"
#include "tables.h"
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
  , brand(_("Brand"))
  , type(_("Type"))
  , color(_("Color"))
  , alias(_("Alias"))
{
}

void DeviceWin::Render() { 
// It is suppposed to be a modal window so change ImGui::Begin() to BeginPopupModal later on.
  ImGui::Begin(_("Insert new device"), &open);
  name.Render();
  brand.Render();
  type.Render();
  color.Render();
  alias.Render();


  if (ImGui::Button(_("Build Device"))) {
    Device device;
    device.Set<Device>()
      .Model(name.Get())
      .Brand(brand.Get())
      .Type(type.Get());
      // no builder 

    device.GetDevice();
  }
  ImGui::End();
}
