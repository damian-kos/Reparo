#pragma once
#include <vector>

class TextField;

template <typename T>
class Attributes {
public:
  Attributes();
  Attributes(const int& _id);
  void Render();
  std::vector<T>& Get();

private:
  TextField input;
  std::vector<T> attrs;
  bool loaded = false;
};

template<typename T>
inline Attributes<T>::Attributes() { 
  attrs = Database::Select<T>().From().All();
}

template<typename T>
inline Attributes<T>::Attributes(const int& _id) {
  std::string _id_str = std::to_string(_id);
  if constexpr (std::is_same_v<T, Color>) {
    // Smells, we are also using it RelationalField
    attrs = Database::Select<T>("id, color") // Smells we are 
      .From("colors")
      .InnerJoin("model_colors mc ON colors.id = mc.color_id ")
      .Where("mc.model_id = " + _id_str).All();
  }
  else if constexpr (std::is_same_v<T, Alias>) {
    attrs = Database::Select<T>().From().Where("model_id = " + _id_str).All();
  }
}

template<typename T>
inline void Attributes<T>::Render() {
  static bool _reload = false;
  static int _to_pop = -1;

  // Only erase if _reload is true and _to_pop is valid
  if (_reload && _to_pop >= 0 && _to_pop < attrs.size()) {
    attrs.erase(attrs.begin() + _to_pop);
    _reload = false; // Reset _reload flag after deleting the item
    _to_pop = -1; // Reset _to_pop after use
  }

  ImGui::PushID(typeid(T).name());
  if (!attrs.empty()) {
    int _index = 0; // Track the _index manually
    for (auto& _attr : attrs) {
      std::string name = _attr.name;

      if (ImGui::BeginTable("Attrs", 2, ImGuiTableFlags_None, ImVec2(150, 20))) {

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("%s", name.c_str());

        ImGui::PushID(name.c_str());
        ImGui::TableNextColumn();
        if (ImGui::Button("x")) {
          _to_pop = _index;
          _reload = true;
        }
        ImGui::PopID();

        ++_index;
        ImGui::EndTable();
      }
    }
  }
  ImGui::Button(_("Add"));
  if (ImGui::BeginPopupContextItem("Attrs", ImGuiPopupFlags_MouseButtonLeft)) {
    input.Render();
    if (ImGui::Button(_("Save"))) {
      T _new_attr{ 0, input.Get() };
      attrs.emplace_back(_new_attr);
      ;
    }
    ImGui::EndPopup();
  }
  ImGui::PopID();
}

template<typename T>
inline std::vector<T>& Attributes<T>::Get() {
  return attrs;
}
