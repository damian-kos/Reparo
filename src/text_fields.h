#pragma once
#include "imgui.h"
#include "imgui_stdlib.h"
#include <string>
#include <vector>

enum ValidatorFlags_ {
  ValidatorFlags_Pass           = 0,
  ValidatorFlags_Fail           = 1 << 0,
  ValidatorFlags_StrLen         = 1 << 1,
  ValidatorFlags_NotEmail       = 1 << 2,
  ValidatorFlags_NotInDb        = 1 << 4,
  ValidatorFlags_IsEmpty        = 1 << 5,
  ValidatorFlags_IsDuplicate    = 1 << 6,
};


enum TFFlags_ {
  TFFlags_None = 0,
  TFFlags_HasPopup = 1 << 0,
  TFFlags_HasRelatedField = 1 << 1,
  TFFlags_HasValidator = 1 << 2,
};

typedef int ValidatorFlags;
typedef int TFFlags;

class Validator {
public:
  static ValidatorFlags DatabaseChk(const std::string&, const std::string& table);
  static ValidatorFlags StrLen(const std::string& buffer, const int& min_len);
  static ValidatorFlags IsEmail(const std::string& buffer);
};

class Popup {
public:
  static bool OnTextInput(std::string& buffer, const std::vector<std::string>& data);
};

class TextField {
public:
  TextField();
  TextField(const std::string& label, ImGuiInputTextFlags = 0, TFFlags = 0);
  virtual void Render();
  const std::string& Get() const;
  const void Clear();

protected:
  std::string label;
  std::string buffer;
  ImGuiInputTextFlags flags = 0;
  TFFlags ro_flags = 0;
  std::string err_msg;

};

class PhoneField : public TextField {
public:
  using TextField::TextField;
  void Render() override;
  
private:
  ValidatorFlags err_flags = 0;

};

class NameField : public TextField {
public:
  using TextField::TextField;
  void Render() override;

private:
  ValidatorFlags err_flags = 0;

};

class SurnameField : public TextField {
public:
  using TextField::TextField;
  void Render() override;

private:
  ValidatorFlags err_flags = 0;

};

class EmailField : public TextField {
public:
  using TextField::TextField;
  void Render() override;

private:
  ValidatorFlags err_flags = 0;

};