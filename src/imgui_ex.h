#pragma once
#include "imgui.h"
#include <string>

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

  void inline BeginWarning(const bool& err) {
    if (err)
      ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0, 0.9, 0.0, 0.5));
  }

  void inline EndColor(const bool& err) {
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

  bool inline ButtonDisabled(const std::string& label, bool disabled) {
    ImGui::BeginDisabled(disabled);
    bool _pressed = ImGui::Button(label.c_str());
    ImGui::EndDisabled();
    return _pressed;
  }

  bool inline ColorButtonEx(const std::string& label, const float color, const bool is_disabled = false) {
    ImGui::PushStyleColor(ImGuiCol_Button,
                          (ImVec4)ImColor::HSV(color, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          (ImVec4)ImColor::HSV(color, 0.7f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          (ImVec4)ImColor::HSV(color, 0.8f, 0.8f));
    ImGui::BeginDisabled(is_disabled);
    bool pressed = ImGui::Button(label.c_str());
    ImGui::EndDisabled();
    ImGui::PopStyleColor(3);
    return pressed;
  }

  void inline TextWrappedColor(const bool _push, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (_push)
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.9, 0.0, 0.5));
    ImGui::TextWrappedV(fmt, args);
    if (_push)
      ImGui::PopStyleColor();
    va_end(args);
  }
}