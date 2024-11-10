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

void RoTable::Addresses(std::vector<std::string>& first, std::vector<std::string>& second) {
  if (ImGui::BeginTable("Address", 2)) {
    // Determine the maximum size of both vectors
    size_t max_size = std::max(first.size(), second.size());

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
