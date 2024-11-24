#pragma once
#include "imgui.h"

namespace ImGui {
  void inline HelpMarker(const char* desc) {
    {
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip())
      {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
    }
  }

  void inline BeginValid(const bool& err) {
    if (err) 
      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.8, 0.5, 0.5, 0.5));
  }

  void inline EndValid(const bool& err) {
    if (err)
      ImGui::PopStyleColor();
  }

  template<typename T>
  void inline View(const T& model) {
    model.View();
  }

  void inline SeparatorColor(const std::string& label, const bool error) {
    if (error) {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    else {
      ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }
    ImGui::PushItemWidth(100);
    ImGui::SeparatorText(label.c_str());

    ImGui::PopStyleColor(1);
    ImGui::PopItemWidth();

  }

  bool inline ButtonDisabled(const char* label, bool disabled) {
    ImGui::BeginDisabled(disabled);
    bool _pressed = ImGui::Button(label);
    ImGui::EndDisabled();
    return _pressed;
  }
}