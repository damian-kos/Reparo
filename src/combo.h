#pragma once
#include <string>

class Brand;
class DeviceWin;

template <typename T>
class RoCombo {
public:
  //RoCombo();
  RoCombo(const std::string& label);
  void Render();
  T& Get();
  void SetLabel(const std::string& _label);
private:
  T model;
  std::string label;
};

template<typename T>
inline RoCombo<T>::RoCombo(const std::string& label) : label(label) {
}

template<typename T>
inline void RoCombo<T>::Render() {
  static std::string _choice;
  label = _choice.empty() ? label : _choice;
  ImGui::PushID(typeid(T).name());
  ImGui::Button(label.c_str());
  if (ImGui::BeginPopupContextItem(typeid(T).name(), ImGuiPopupFlags_MouseButtonLeft)) {
    static std::vector<T> _choices = Database::Select<T>().From().All();
    static int _sel = 0;

    if (ImGui::BeginCombo(label.c_str(), _choices[_sel].name.c_str())) {
      for (size_t i = 0; i < _choices.size(); ++i) {
        const bool _is_sel = (_sel == i);
        if (ImGui::Selectable(_choices[i].name.c_str(), _is_sel)) {
          _sel = i;
          _choice = _choices[i].name;
          model = _choices[i];
        }
        if (_is_sel)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::EndPopup();
  }
  ImGui::PopID();
}

template<typename T>
inline T& RoCombo<T>::Get() {
  return model;
}

template<typename T>
inline void RoCombo<T>::SetLabel(const std::string& _label) {
  if (_label.empty()) { return; }
  label = _label;
}