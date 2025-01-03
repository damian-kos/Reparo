#include "filters.h"
#include "imguidatechooser.h"
#include <iostream>
#include "conversion.h"

DateFilter::DateFilter() 
  : directions{
    { 1, "Before"},
    { 2, "After"},
    { 3, "Between"}
  }
  {
  ImGui::SetDateZero(&from);
  ImGui::SetDateZero(&to);
}

bool DateFilter::Render() {
  bool _state = false;
  ImGui::PushItemWidth(250);
  if (ImGui::BeginCombo("##Relation", directions[selected].c_str())) {
    for (auto& pair : directions) {
      if (ImGui::Selectable(pair.second.c_str())) {
        selected = pair.first;
        _state = true;
      }
    }
    ImGui::EndCombo();
  }
  ImGui::SameLine();
  if (ImGui::DateChooser("##From", from)) {
    _state = true;
    std::cout << Convert::TmToStr(from) << std::endl;
  }
  ImGui::SameLine();
  ImGui::BeginDisabled(selected != 3);
  if (ImGui::DateChooser("##To", to)) {
    _state = true;
    std::cout << Convert::TmToStr(to) << std::endl;
  }
  ImGui::EndDisabled();
  ImGui::PopItemWidth();
  return _state;
}

std::string DateFilter::GetForSQL() {
  std::string _string_for_sql;
  switch (selected) {
    case 1:
      _string_for_sql = " < '" + Convert::TmToStr(from) + "'";
      break;
    case 2:
      _string_for_sql = " > '" + Convert::TmToStr(from) + "'";
      break;
    case 3:
      _string_for_sql = " BETWEEN '" + Convert::TmToStr(from) + "' AND '" + Convert::TmToStr(to) + "'";
      break;
    default:
      _string_for_sql = "";
      break;
  }
  return _string_for_sql;
}
