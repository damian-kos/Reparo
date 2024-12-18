#pragma once
#include <string>
#include "imgui.h"

class DeviceWin;

template <typename T>
class RoCombo {
public:
  //RoCombo();
  RoCombo(const std::string& _label = "Empty Combo");
  RoCombo(const std::vector<T> _models);
  void RenderFromBtn();
  bool Render();
  T& Get();
  void SetLabel(const std::string& _label);
private:
  T model;
  std::vector<T> models;
  std::string label;
};

template<typename T>
inline RoCombo<T>::RoCombo(const std::string& _label) : label(_label) {
  models = Database::Select<T>().From().All();
  if(!models.empty())
    model = models[0];
}

template<typename T>
inline RoCombo<T>::RoCombo(const std::vector<T> _models) : models(_models) {
  // Currently there is no need to have this described with label.
  // Combos are self explanatory
  label = "##combo";
  if (!models.empty())
    model = models[0];
}

template<typename T>
inline void RoCombo<T>::RenderFromBtn() {
  std::string _btn;
  _btn = model.name.empty() ? label : model.name;
  ImGui::PushID(typeid(T).name());
  if (ImGui::Button(_btn.c_str()))
    ImGui::OpenPopup(typeid(T).name());

  if (ImGui::BeginPopup(typeid(T).name())) {
    if(Render())
      ImGui::CloseCurrentPopup();
    
    ImGui::EndPopup();
  }

  ImGui::PopID();
}

template <typename T> inline bool RoCombo<T>::Render() {
  static int _sel = 0;
  bool _set = false;
  if (ImGui::BeginCombo(label.c_str(), models[_sel].name.c_str())) {
    for (size_t i = 0; i < models.size(); ++i) {
      const bool _is_sel = (_sel == i);
      if (ImGui::Selectable(models[i].name.c_str(), _is_sel)) {
        _sel = i;
        model = models[i];
        _set = true;
      }
    }
    ImGui::EndCombo();
  }
  return _set;
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

template <typename T>
class RoTabBar {
public:
  RoTabBar(const std::vector<T> _models);
  bool Render();
  T Get();
private:
  std::vector<T> models;
  std::string label = "TabBar";
  T model;
};

template<typename T>
inline RoTabBar<T>::RoTabBar(const std::vector<T> _models) :models(_models) { }

template<typename T>
inline bool RoTabBar<T>::Render() {
  bool _state = false;
  static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_FittingPolicyScroll;
  if (ImGui::BeginTabBar(label.c_str(), tab_bar_flags)) {
    for (auto& item : models) {
      if (ImGui::BeginTabItem(item.name.c_str())) {
        ImGui::EndTabItem();
      }
      if (ImGui::IsItemClicked()) {
        model = item;
        _state = true;
      }
    }
    ImGui::EndTabBar();
  }
  return _state;
}

template<typename T>
inline T RoTabBar<T>::Get() {
  return model;
}
