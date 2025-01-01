#include "menu_bar.h"
#include "imgui.h"
#include "RoLocalization.h"
#include "reparo_windows.h"

RoMenuBar::RoMenuBar() {
}

void RoMenuBar::Render() {
  if (!ImGui::BeginMainMenuBar()) {
    return;
  }
  if (ImGui::BeginMenu(_("New"))) {
    Customer();
    ImGui::EndMenu();
  }
  ImGui::EndMainMenuBar();
}

void RoMenuBar::Customer() {
  if (ImGui::BeginMenu(_("Customer"))) {
    if (ImGui::MenuItem(_("New Customer"))) {
      WindowFactory::AddWindow("customer");
    }
    ImGui::SeparatorText(_("Options"));
    if (ImGui::MenuItem(_("Edit Customer"))) {
      // Do something
    }
    ImGui::EndMenu();
  }
}

