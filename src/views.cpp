#include "views.h"
#include <iostream> 
#include "RoLocalization.h"
#include "conversion.h"

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

void RepairView::DefaultRenderItem(const Repair& _repair) {
  ImGui::TableNextColumn();
  std::string _id_str = std::to_string(_repair.id);
  ImGui::Selectable(_id_str.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.phone.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.device.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.category.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.vis_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.hid_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _repair.price);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.repair_state.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.sn_imei.c_str());

  ImGui::TableNextColumn();
  std::string _created_at = Convert::TmToStr(_repair.created_at);
  ImGui::Text("%s", _created_at.c_str());

  ImGui::TableNextColumn();
  std::string _updated_at = Convert::TmToStr(_repair.updated_at);
  ImGui::Text("%s", _updated_at.c_str());

  ImGui::TableNextColumn();
  std::string _finished_at = Convert::TmToStr(_repair.finished_at);
  ImGui::Text("%s", _finished_at.c_str());

}
