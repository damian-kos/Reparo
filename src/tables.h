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
    int i = 1;
    for (auto& model : models) {

      ImGui::TableNextColumn();
      char label[32];
      int _id = model.id > 0 ? model.id : i;
      sprintf_s(label, "%d", _id);
      ImGui::Selectable(label, false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups);
      if (ImGui::BeginPopupContextItem()) {

        model.EditModal();
        model.DeleteModal();

        ImGui::EndPopup();
      }
      ImGui::TableNextColumn();
      ImGui::Text("%s", model.name.c_str());
      ++i;
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



