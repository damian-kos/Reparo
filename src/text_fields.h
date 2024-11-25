#pragma once
#include "imgui.h"
#include <imgui_stdlib.h>

#include <string>
#include <vector>
#include "../../src/models/device.h"
#include "../../src/models/customer.h"
#include "imgui_ex.h"
#include "database.h"

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
  TFFlags_None = 0,                    // 
  TFFlags_HasPopup = 1 << 0,           // Set if field is supposed to have its Popup
  TFFlags_HasRelatedField = 1 << 1,    // Set if field data in Popup should be affected by related field
  TFFlags_HasValidator = 1 << 2,       // Set if field is supposed to run validations
  TFFlags_AllowDbPresence = 1 << 3,    // Set if found result in Database shouldn't result in error
};

typedef int ValidatorFlags;
typedef int TFFlags;

class Validator {
public:
template <typename T>
  static ValidatorFlags DatabaseChk(const std::string& table, const std::string& condition);
  static ValidatorFlags StrLen(const std::string& buffer, const int& min_len);
  static ValidatorFlags IsEmail(const std::string& buffer);
};

class Popup {
public:
  template <typename T>
  static T OnTextInput(std::string& buffer, const std::vector<T>& data = {});
};

class TextField {
public:
  TextField();
  TextField(const std::string& label, ImGuiInputTextFlags = 0, TFFlags = 0);
  int Render();
  virtual void Feedback();
  virtual void Validate();
  const std::string& Get() const;
  const void Clear();
  void FillBuffer(const std::string& fill);
  bool error = true; // trivial so no getter
protected:
  std::string label;
  std::string buffer;
  ImGuiInputTextFlags flags = 0;
  TFFlags ro_flags = 0;
  ValidatorFlags err_flags = 0;
  bool has_error_with_content = false;
};

class PhoneField : public TextField {
public:
  using TextField::TextField;
  Customer Render();
  void Validate() override;
  void Feedback() override;
  Customer GetFromDb();
};

class NameField : public TextField {
public:
  using TextField::TextField;
  int Render();
  void Validate() override;
  void Feedback() override;

};

class SurnameField : public TextField {
public:
  using TextField::TextField;
  int Render();
  void Validate() override;
  void Feedback() override;

};

class EmailField : public TextField {
public:
  using TextField::TextField;
  int Render();
  void Validate() override;
  void Feedback() override;

};

class DeviceField : public TextField {
public:
  using TextField::TextField;
  Device& Render();
  void Validate() override;
  void Feedback() override;
  Device GetFromDb();
private:
  Device device;
};

template <typename SM>
class SimpleModelField : public TextField {
public:
  using TextField::TextField;
  SM& Render();
  void Validate() override;
  void Feedback() override;
  SM GetFromDb();
};

template <typename SM, typename R>
class RelationalField : public TextField {
public:
  using TextField::TextField;
  SM& Render(const R& related);
  void Validate() override;
  void Feedback() override;
  SM GetFromDb();
};
