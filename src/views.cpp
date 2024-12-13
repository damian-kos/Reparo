#include "views.h"
#include <iostream> 
#include "RoLocalization.h"

void CustomerView::DefaultRenderItem(const Customer& customer) {
  ImGui::TableNextColumn();
  ImGui::Text("%d", customer.id);

  ImGui::TableNextColumn();

  if (customer.HasRepairs()) {
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.3f, 0.3f, 0.65f));
  }

  ImGui::Selectable(customer.phone.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  if (customer.HasRepairs()) {
    ImGui::PopStyleColor();
    ImGui::SetItemTooltip("Can't remove customer as long as there is a related repair.", ImGui::GetStyle().HoverDelayShort);
  }
  else {
    if (ImGui::BeginPopupContextItem()) {


      if (ImGui::Button(_("Remove customer?")))
        customer.RemoveModal();

      ImGui::EndPopup();
    }
  }

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.name.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.surname.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.email.c_str());

}

