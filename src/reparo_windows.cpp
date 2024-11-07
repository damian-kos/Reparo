#include "reparo_windows.h"
#include "imgui.h"
#include "RoLocalization.h"
#include "models/customer.h"

CustomerWin::CustomerWin()
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup)
  , name(_("Name"))
  , surname(_("Surname"))
  , email(_("Email"))
{}

void CustomerWin::Render() {
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
    customer.Get();
  }
  ImGui::End();
}
