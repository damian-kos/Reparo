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

void RoTable::Addresses(const std::vector<std::string>& first, std::string _label) {
  if (ImGui::BeginTable(_label.c_str(), 1, 0, ImVec2(250, 0))) {
    size_t max_size = first.size();

    ImGui::TableSetupColumn(_label.c_str());
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
    }
    ImGui::EndTable();
  }
}

void RoTable::Addresses(const std::vector<std::string>& _first, std::string _label, const std::vector<std::string>& _second) {
  if (ImGui::BeginTable(_label.c_str(), 1, 0, ImVec2(250,0))) {
    size_t max_size = _first.size();

    ImGui::TableSetupColumn(_label.c_str());
    ImGui::TableHeadersRow();

    for (size_t i = 0; i < max_size; i++) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      if (i < _first.size()) {
        ImGui::TextWrappedColor(_first[i] != _second[i], "%s", _first[i].c_str());
      }
      else {
        ImGui::Text("");  // Empty string for missing data
      }
    }
    ImGui::EndTable();
  }
}

void RoTable::TableWithDevices(const std::vector<Device>& _devices, std::vector<Device>& _cmptbl_devices, std::vector<Alias>& _cmptbl_aliases) {
  float window = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  ImVec2 size = ImGui::CalcTextSize("Apple Watch Series 2 Aluminium 38mm");
  int columns = window / (size.x * 1.2) - 1;

  if (columns <= 0)
    return;

  if (!ImGui::BeginTable("split1", columns, ImGuiTableFlags_SizingFixedFit |
    ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders |
    ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame))
    return;

  for (auto& device : _devices) {
    ImGui::TableNextColumn();
    if (device.name.empty())
      continue;

    if (ImGui::Selectable(device.name.c_str(), false, ImGuiSelectableFlags_DontClosePopups)) {
      // Remove alias entries linked to the device id
      _cmptbl_aliases.erase(std::remove_if(_cmptbl_aliases.begin(), _cmptbl_aliases.end(), [&](const Alias& alias) {
        return device.id == alias.link_id;
        }), _cmptbl_aliases.end());

      // Check if device is already in _cmptbl_devices (by device id)
      auto device_it = std::find_if(_cmptbl_devices.begin(), _cmptbl_devices.end(), [&](const Device& d) {
        return d.id == device.id;
        });

      if (device_it == _cmptbl_devices.end()) { // Device not found, insert it
        _cmptbl_devices.push_back(device);
      }
    }

    if (!ImGui::BeginPopupContextItem())
      continue;

    if (device.aliases.empty()) {
      ImGui::EndPopup();
      continue;
    }

    for (size_t i = 0; i < device.aliases.size(); ++i) {
      auto& alias = device.aliases[i];

      if (!ImGui::Selectable(alias.name.c_str(), false, ImGuiSelectableFlags_DontClosePopups))
        continue;

      // Remove device entries linked to alias
      _cmptbl_devices.erase(std::remove_if(_cmptbl_devices.begin(), _cmptbl_devices.end(), [&](const Device& d) {
        return d.id == alias.link_id;
        }), _cmptbl_devices.end());

      // Check if alias is already in _cmptbl_aliases (by alias id)
      auto alias_it = std::find_if(_cmptbl_aliases.begin(), _cmptbl_aliases.end(), [&](const Alias& a) {
        return a.id == alias.id;
        });

      if (alias_it == _cmptbl_aliases.end()) { // Alias not found, insert it
        _cmptbl_aliases.push_back(alias);
      }
    }

    ImGui::EndPopup();
  }

  ImGui::EndTable();
}

void RoTable::SideBySideText(const std::string&  _label, const std::vector<ModelDataView>& _data) {
  ImGui::SeparatorText(_label.c_str());
  if (ImGui::BeginTable(_label.c_str(), 2, ImGuiTableFlags_RowBg)) {
    for (auto& _record: _data) {
      ImGui::TableNextColumn();
      ImGui::TextWrapped("%s", _record.label.c_str());
      ImGui::TableNextColumn();
      ImGui::TextWrappedColor(_record.modified, "%s", _record.data.c_str());
      if(_record.modified)
        ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg1, ImGui::GetColorU32(ImVec4(1.0, 0.9, 0.0, 0.1)));
    }
    ImGui::EndTable();
  }
}


