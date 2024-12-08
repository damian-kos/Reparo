#pragma once
#include <vector>
#include "text_fields.h"
#include "modal.h"
#include "models/device.h"

class RoTable {
public:
  static void AddressesInputs(std::vector<TextField>& first, std::vector<TextField>& second);
  static void Addresses(const std::vector<std::string>& first, const std::vector<std::string>& second);
  template <typename T>
  static void SimpleModel(std::vector<T>& models);
  static void TableWithDevices(const std::vector<Device>& _devices, std::unordered_map<int, Device>& _cmptbl_devices, std::unordered_map<int, Alias>& _cmptbl_aliases);
  template <typename T>
  static void TableWrapped(const T& _values);

 private:
  template <typename T>
  static void ProcessIterators(const std::vector<T>& _values);
  template <typename T>
  static void ProcessIterators(const std::unordered_map<int, T>& _values);
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
          ModalConfig config;
          config.Title(_("Edit this record?"))
            .Msg(_("All the elements with this record will be changed as well."));
          SimpleModelModal<T> modal(model, config);
          StackModal::SetModal(modal);
        }
        // This modal builder is here so we can have access to specific records without returning them
        // Figure out how to put them in SimpleModalWin
        if (ImGui::Button("Delete")) {
          ModalConfig config;
          config.Title(_("Delete this record?"))
            .Msg(_("Are you sure? All records which are including this record will be set to Unknown."));
          SimpleModelModal<T> modal(model, config);
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

template <typename T>
inline void RoTable::ProcessIterators(const std::vector<T>& _values) {
  for (auto& value : _values) {
    ImGui::TableNextColumn();
    ImGui::Text("%s", value.name.c_str());
  }
}

template <typename T>
inline void RoTable::ProcessIterators(
    const std::unordered_map<int, T>& _values) {
  for (auto& [key, value] : _values) {
    ImGui::TableNextColumn();
    ImGui::Text("%s", value.name.c_str());
  }
}

template <typename T>
inline void RoTable::TableWrapped(const T& _values) {
  float window = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  ImVec2 size = ImGui::CalcTextSize("Apple Watch Series 2 Aluminium 38mm");
  int columns = window / (size.x * 1.2) - 1;
  int height = size.y * 5;

  if (columns > 0 && _values.size() > 0) {
    if (ImGui::BeginTable("split1", columns,
                          ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders |
                              ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_SizingStretchSame,
                          ImVec2(0, height))) {
      
      ProcessIterators(_values);

      ImGui::EndTable();
    }
  }
}



