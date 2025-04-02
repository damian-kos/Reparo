#include "buttons.h"

Buttons::Buttons(const std::string& _label, float _color) : label(_label), color(_color) {
}

void Buttons::Render(bool _disabled) {
  ImGui::BeginDisabled(_disabled);
  pressed = ImGui::ColorButtonEx(label, 0.2f, _disabled);
  ImGui::EndDisabled();
}
