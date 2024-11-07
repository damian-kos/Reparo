#include "text_fields.h"
#include "imgui.h"
#include <iostream>
#include "imgui_ex.h"
#include <algorithm> // to delete, just used for std::find()

ValidatorFlags Validator::DatabaseChk(const std::string&, const std::string& table) {
  return ValidatorFlags_IsDuplicate;
}

ValidatorFlags Validator::StrLen(const std::string& buffer, const int& min_len){
  if (buffer.size() > 0 && buffer.size() < min_len)
    return ValidatorFlags_StrLen;
  return ValidatorFlags_Pass;
}

ValidatorFlags Validator::IsEmail(const std::string& buffer) {
  size_t atPos = buffer.find('@');
  size_t dotPos = buffer.find('.', atPos + 1);
  if (atPos != std::string::npos && dotPos != std::string::npos) {
    return ValidatorFlags_Pass;
  }
  return ValidatorFlags_NotEmail;
}

bool Popup::OnTextInput(std::string& buffer, const std::vector<std::string>& data = std::vector<std::string>()) {
    float x = ImGui::GetWindowContentRegionMax().x;
    bool closed = false;
    bool is_input_active = ImGui::IsItemActive();
    bool is_input_activated = ImGui::IsItemActivated();

    if (is_input_activated) {
      ImGui::OpenPopup("Popup");

    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
      if (ImGui::BeginPopup("Popup", ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_ChildWindow))
    {

      ImVec2 scrolling_child_size = ImVec2(x - 30, ImGui::GetFrameHeightWithSpacing() * 3 + 30);
      ImGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
      ImGui::HelpMarker("Click auto fill with selection.");
      for (auto& record : data) {
        //std::string concatenated = record.first + record.second;
        if (ImGui::Selectable(record.c_str())) {
          buffer = record;
          closed = true;
          ImGui::CloseCurrentPopup();
        }
      }
      if (!is_input_active && !ImGui::IsWindowFocused()) {
        std::cout << "Close current popup" << std::endl;
        closed = true;
        ImGui::CloseCurrentPopup();
      }
      float scroll_x = ImGui::GetScrollX();
      float scroll_max_x = ImGui::GetScrollMaxX();
      ImGui::EndChild();
      ImGui::EndPopup();
    }
    return closed;
  }

TextField::TextField(const std::string& label, ImGuiInputTextFlags flags, TFFlags ro_flags)
  : label(label), flags(flags), ro_flags(ro_flags) { }

void TextField::Render() {
  ImGui::InputTextWithHint(("##" + label).c_str(), label.c_str(), &buffer, flags);
}

void PhoneField::Render() {
  const bool err = err_flags & (ValidatorFlags_StrLen | ValidatorFlags_IsDuplicate);

  ImGui::BeginValid(err);
  ImGui::InputTextWithHint(("##" + label).c_str(), label.c_str(), &buffer, flags);
  ImGui::EndValid(err);

  if (ImGui::IsItemEdited()) {
    err_flags = ValidatorFlags_Pass;  
    err_flags |= Validator::DatabaseChk(buffer, ""); // Edit with corresponding table once data in db exists
    err_flags |= Validator::StrLen(buffer, 7);
  }

#ifdef DEBUG
  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HAS_POPUP);
    Popup::OnTextInput(buffer);
  if (err_flags & ValidatorFlags_StrLen)
    ImGui::Text("Phone too short");
  if (err_flags & ValidatorFlags_IsDuplicate)
    ImGui::Text("Phone already exists");
  ImGui::PopID();
#else 
  if (ro_flags & TFFlags_HAS_POPUP)
    Popup::OnTextInput(buffer);
#endif // DEBUG
}

void NameField::Render() {
  const bool err = err_flags & ValidatorFlags_StrLen;

  ImGui::BeginValid(err);
  ImGui::InputTextWithHint(("##" + label).c_str(), label.c_str(), &buffer, flags);
  ImGui::EndValid(err);

  if (ImGui::IsItemEdited()) {
    err_flags = ValidatorFlags_Pass;  // Reset flags
    err_flags |= Validator::StrLen(buffer, 3);
  }

  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HAS_POPUP)
    Popup::OnTextInput(buffer);
  ImGui::PopID();

#ifdef DEBUG
  if (err_flags & ValidatorFlags_StrLen)
    ImGui::Text("Name too short");
#endif // DEBUG
}

void SurnameField::Render() {
  const bool err = err_flags & ValidatorFlags_NotEmail;

  ImGui::BeginValid(err);
  ImGui::InputTextWithHint(("##" + label).c_str(), label.c_str(), &buffer, flags);
  ImGui::EndValid(err);

  if (ImGui::IsItemEdited()) {
    err_flags = ValidatorFlags_Pass;  // Reset flags
    err_flags |= Validator::StrLen(buffer, 3);
  }

  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HAS_POPUP)
    Popup::OnTextInput(buffer);
  ImGui::PopID();

#ifdef DEBUG
  if (err_flags & ValidatorFlags_StrLen) 
    ImGui::Text("Surname too short");
#endif // DEBUG
}

void EmailField::Render() {
  const bool err = err_flags & ValidatorFlags_NotEmail;

  ImGui::BeginValid(err);
  ImGui::InputTextWithHint(("##" + label).c_str(), label.c_str(), &buffer, flags);
  ImGui::EndValid(err);

  if (ImGui::IsItemEdited()) {
    err_flags = ValidatorFlags_Pass;  // Reset flags
    err_flags |= Validator::IsEmail(buffer);
  }

  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HAS_POPUP)
    Popup::OnTextInput(buffer);
  ImGui::PopID();

#ifdef DEBUG
  if (err_flags & ValidatorFlags_NotEmail)
    ImGui::Text("Invalid email");
#endif // DEBUG
}