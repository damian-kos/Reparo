#include "reparo_windows.h"
#include "imgui.h"
#include "RoLocalization.h"
#include "modal.h"
#include "models/customer.h"
#include "database.h"

CustomerWin::CustomerWin()
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup)
  , name(_("Name"))
  , surname(_("Surname"))
  , email(_("Email"))
{}

void CustomerWin::Render() {
  static Modal<Customer> modal;

  ImGui::Begin(_("Insert Customer"), &open);
  phone.Render();
  name.Render();
  surname.Render();
  email.Render();
  if (ImGui::Button(_("Submit Customer Details"))) {
    Customer customer;
    
    customer.Set()
      .Phone(phone.Get())
      .Name(name.Get())
      .Surname(surname.Get())
      .Email(email.Get());

    Config<Customer> config{
      .title = _("Insert Customer?"),
      .msg = _("Are you sure?"),
      .caller = customer.GetCustomer(),
    };
    Database::Insert().Customer_(customer); // move to modal
    modal.SetConfig(config);
  }
  modal.Render();

  ImGui::End();
}
