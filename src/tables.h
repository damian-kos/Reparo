#pragma once
#include <vector>
#include "text_fields.h"
#include "modal.h"

class RoTable {
public:
  static void AddressesInputs(std::vector<TextField>& first, std::vector<TextField>& second);
  static void Addresses(const std::vector<std::string>& first, const std::vector<std::string>& second);
  template <typename T>
  static void SimpleModel(std::vector<T>& models);
};

template<typename T>
inline void RoTable::SimpleModel(std::vector<T>& models) {
  if (ImGui::BeginTable("IDValue", 2)) {
    ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_NoHide);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_NoHide);
    ImGui::TableHeadersRow();
    ImGui::TableNextRow();
    for (auto& model : models) {
      static int selected = -1;
      const bool is_selected = (selected == model.id);

      ImGui::TableNextColumn();
      char label[32];
      sprintf_s(label, "%d", model.id);
      if (ImGui::Selectable(label, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups)) {
      }

      if (ImGui::BeginPopupContextItem()) {
        // This modal builder is here so we can have access to specific records without returning them
        // Figure out how to put them in SimpleModalWin
        if (ImGui::Button(_("Edit record"))) {
          BuildSimpleModelModal<T> build;
          build.Title(_("Edit this record?"))
            .Message(_("All the elements with this record will be changed as well."))
            .SetSimpleModel(model);

          SimpleModelModal<T> modal(build.Build());
          StackModal::SetModal(modal);
        }
        // This modal builder is here so we can have access to specific records without returning them
        // Figure out how to put them in SimpleModalWin
        if (ImGui::Button("Delete")) {
          BuildSimpleModelModal<T> build;
          build.Title(_("Delete this record?"))
            .Message(_("Are you sure? All records which are including this record will be set to Unknown."))
            .SetSimpleModel(model);

          SimpleModelModal<T> modal(build.Build());
          StackModal::SetModal(modal);
        }
        ImGui::EndPopup();
      }
      ImGui::TableNextColumn();
      ImGui::Text("%s", model.name.c_str());
    }
  ImGui::EndTable();
  }
}
