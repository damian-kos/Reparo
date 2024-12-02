#include "tables.h"
#include "imgui.h"
#include <iostream>

void RoTable::AddressesInputs(std::vector<TextField>& first, std::vector<TextField>& second) {
  if (ImGui::BeginTable("Address", 2)) {
    static size_t vec_size = first.size();
    for (int i = 0; i < vec_size; i++) {
      ImGui::TableNextColumn();
      first[i].Render();
      ImGui::TableNextColumn();
      second[i].Render();
    }
    ImGui::EndTable();
  }
}

void RoTable::Addresses(const std::vector<std::string>& first, const std::vector<std::string>& second) {
  if (ImGui::BeginTable("Address", 2)) {
    size_t max_size = (std::max)(first.size(), second.size());

    static std::vector<std::string> col_names = { "Billing Address", "Shipping Address" };
    for (auto& name : col_names)
      ImGui::TableSetupColumn(name.c_str());
    ImGui::TableHeadersRow();
    
    for (size_t i = 0; i < max_size; i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      if (i < first.size()) {
        ImGui::Text("%s", first[i].c_str());
      }
      else {
        ImGui::Text("");  // Empty string for missing data
      }

      ImGui::TableNextColumn();
      if (i < second.size()) {
        ImGui::Text("%s", second[i].c_str());
      }
      else {
        ImGui::Text("");  // Empty string for missing data
      }
    }
    ImGui::EndTable();
  }
}

void RoTable::TableWithDevices(const std::vector<Device>& _devices, std::unordered_map<int, Device>& _cmptbl_devices, std::unordered_map<int, Alias>& _cmptbl_aliases) {
  float window = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  ImVec2 size = ImGui::CalcTextSize("Apple Watch Series 2 Aluminium 38mm");
  int columns = window / (size.x * 1.2) - 1;

  if (columns <= 0)
    return;

  if (!ImGui::BeginTable("split1", columns, ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit |
    ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders |
    ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame))
    return;

  for (auto& device : _devices) {
    if (device.name.empty())
      continue;

    ImGui::TableNextColumn();

    if (ImGui::Selectable(device.name.c_str())) {
      std::erase_if(_cmptbl_aliases, [&](const auto& pair) {
        return device.id == pair.second.link_id;
        });
      _cmptbl_devices.emplace(device.id, device);
    }

    if (!ImGui::BeginPopupContextItem())
      continue;

    if (device.aliases.empty()) {
      ImGui::EndPopup();
      continue;
    }

    static std::vector<bool> selection(device.aliases.size(), false);

    for (size_t i = 0; i < device.aliases.size(); ++i) {
      auto& alias = device.aliases[i];

      if (!ImGui::Selectable(alias.name.c_str(), selection[i], ImGuiSelectableFlags_DontClosePopups))
        continue;

      std::erase_if(_cmptbl_devices, [&](const auto& pair) {
        return pair.first == alias.link_id;
        });

      // Older solution left as on above possibly caused a vector out-of-range bug ?
      //auto link_it = _cmptbl_devices.find(alias.link_id);
      //if (link_it != _cmptbl_devices.end())
      //  _cmptbl_devices.erase(link_it);

      _cmptbl_aliases.emplace(alias.id, alias);
    }

    ImGui::EndPopup();
  }

  ImGui::EndTable();
}
