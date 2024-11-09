#pragma once
#include "imgui.h"

namespace ImGui {
  void HelpMarker(const char* desc) {
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

  void BeginValid(const bool& err) {
    if (err) 
      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.8, 0.5, 0.5, 0.5));
  }

  void EndValid(const bool& err) {
    if (err)
      ImGui::PopStyleColor();
  }


  template<typename T>
  void View(const T& model) {
    model.View();
  }
}