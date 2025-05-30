#pragma once
#include <string>
#include "imgui.h"
#include "RoLocalization.h"

enum RoComboFlags_ {
  RoComboFlags_None = 0,
  RoComboFlags_HasNone = 1 << 0, // Some combos may have non meaning value 
};

typedef int RoComboFlags;

template <typename T>
class RoCombo {
public:
  //RoCombo();
  RoCombo(const std::string& _label = "Empty Combo", RoComboFlags _flags = 0);
  RoCombo(T& _model, RoComboFlags _flags = 0);
  RoCombo(const std::vector<T> _models, RoComboFlags _flags = 0);
  void IncludeNone();
  void RenderFromBtn();
  bool Render();
  T& Get();
  void SetLabel(const std::string& _label);
  void SetModel(const T& _model);
private:
  T model;
  std::vector<T> models;
  std::string label;
  RoComboFlags flags;
  int selected = 0;
};

template<typename T>
inline RoCombo<T>::RoCombo(const std::string& _label, RoComboFlags _flags) : label(_label), flags(_flags) {
  models = Database::Select<T>().From().All();
  IncludeNone();

  if constexpr (std::is_same_v<T, RepairState>) {
    // For RepairState, remove the first element if models is not empty
    if (!models.empty()) {
      models.erase(models.begin());
    }
  }

  if(!models.empty())
    model = models[0];
}


template<typename T>
inline RoCombo<T>::RoCombo(T& _model, RoComboFlags _flags) : model(_model), flags(_flags) {
  label = "##combo";
  models = Database::Select<T>().From().All();
}

template<typename T>
inline RoCombo<T>::RoCombo(const std::vector<T> _models, RoComboFlags _flags) : models(_models), flags(_flags) {
  // Currently there is no need to have this described with label.
  // Combos are self explanatory
  label = "##combo";
  if (!models.empty())
    model = models[0];
}

template<typename T>
inline void RoCombo<T>::IncludeNone() {
  if (flags & RoComboFlags_HasNone) {
    T _none;
    _none.id = -1;
    _none.name = "None";
    models.insert(models.begin(), _none);
  }
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
  //static int selected = 0;
  bool _set = false;

  if (models.empty()) {
    ImGui::Text(_("No records found"));
    return false; 
  }

  if (ImGui::BeginCombo(label.c_str(), models[selected].name.c_str())) {
    for (size_t i = 0; i < models.size(); ++i) {
      const bool _is_selected = (selected == i);
      if (ImGui::Selectable(models[i].name.c_str(), _is_selected)) {
        selected = i;
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
  if (model.name == "None") { // So if none is being choosen, the model's data remains empty
    model.name = "";
  }
  return model;
}

template<typename T>
inline void RoCombo<T>::SetLabel(const std::string& _label) {
  if (_label.empty()) { return; }
  label = _label;
}

template<typename T>
inline void RoCombo<T>::SetModel(const T& _model) {
  model = _model;
  for (int i = 0; i < models.size(); ++i) {
    if (models[i].id == _model.id) {
      selected = i;
      break;
    }
  }
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
