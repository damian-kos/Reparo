// modal.h
#pragma once
#include <string>
#include <imgui.h>
#include "models/customer.h"
#include <functional>
#include "RoLocalization.h"
#include "database.h"

template  <typename T>
struct Config {
  std::string title;
  std::string msg;
  T caller;
  T temp_caller;
  std::function<void()> on_confirm;
};

template  <typename T>
class Modal {
public:
  void Render();
  void SetConfig(const Config<T>& _config);
protected:
  Config<T> config;
  bool is_on = false;
};

template<typename T>
inline void Modal<T>::Render() {
  if (!is_on) { return; }
  ImGui::OpenPopup(config.title.c_str());
  if (ImGui::BeginPopupModal(config.title.c_str(), &is_on)) {
    config.caller.View();
    if (ImGui::Button(_("Confirm"))) {
      config.on_confirm();
      ImGui::CloseCurrentPopup();
    }
    if (ImGui::Button(_("Cancel"))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

template<typename T>
inline void Modal<T>::SetConfig(const Config<T>& _config) {
  config = _config;
  is_on = true;
}
