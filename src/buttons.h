#include "imgui.h"
#include "imgui_ex.h"

class Buttons {
public:
  Buttons() {}
  Buttons(const std::string& _label, float _color = 0.0f);
  void Render(bool _disabled = false);
  bool pressed = false;

private:
  std::string label;
  float color;
};

namespace Button {
  bool inline Submit(const std::string& _label, bool _disabled = false) {
    ImGui::BeginDisabled(_disabled);
    bool _pressed = ImGui::ColorButtonEx(_label, 0.2f, _disabled);
    ImGui::EndDisabled();
    return _pressed;
  }
}