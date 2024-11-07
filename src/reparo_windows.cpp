#include "reparo_windows.h"
#include "imgui.h"

CustomerWin::CustomerWin()
  : phone("Phone", ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup)
  , name("Name")
  , surname("Surname")
  , email("Email")
{}

void CustomerWin::Render() {
  ImGui::Begin("Insert Customer", &open);
  phone.Render();
  name.Render();
  surname.Render();
  email.Render();
  ImGui::End();
}
