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
  ValidatorFlags_EmptyIsError   = 1 << 7,

};

enum TFFlags_ {
  TFFlags_None                = 0,        // 
  TFFlags_HasPopup            = 1 << 0,   // Set if field is supposed to have its Popup
  TFFlags_HasRelatedField     = 1 << 1,   // Set if field data in Popup should be affected by related field
  TFFlags_HasValidator        = 1 << 2,   // Set if field is supposed to run validations
  TFFlags_AllowDbPresence     = 1 << 3,   // Set if found result in Database shouldn't result in error
  TFFlags_EmptyIsError        = 1 << 4,   // Empty buffer will set field to have en error if set
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

template <typename T>
class Popup {
public:
  bool OnTextInput(std::string& buffer, const std::vector<T>& data = {});
  bool closed = false;
  int selected = -1;
  T record;
};

class TextField {
public:
  TextField();
  TextField(const std::string& label, ImGuiInputTextFlags = 0, TFFlags = 0);
  virtual ~TextField() {};
  int Render();
  void Field();
  virtual void Feedback();
  void FeedbackEx(std::initializer_list<std::string> args = {});
  virtual void Validate();
  void EmptyBufferError();
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
  bool Render();
  void Validate() override;
  void Feedback() override;
  Customer GetFromDb();
  Customer customer;
private:
  Popup<Customer> popup;
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
  Device Render();
  void Validate() override;
  void Feedback() override;
  Device GetFromDb();
  bool IsInDb();
private:
  Device device;
  Popup<Device> popup;
};

class OwnSKUField : public TextField {
public:
  using TextField::TextField;
  bool Render();
  void Validate() override;
  void Feedback() override;
  Part GetFromDb();
  Part part;
private:
  Popup<Part> popup;
};

class QueriedTextField : public TextField {
public:
  using TextField::TextField;
  QueriedTextField(const std::string& label, ImGuiInputTextFlags = 0, TFFlags = 0
    , const std::string& _select = "*"
    , const std::string& _from   = "_"
    , const std::string& _where  = "_");
  void Render();
  void Validate() override;
  void Feedback() override;

private:
  Popup<std::string> popup;
  std::string select;
  std::string from;
  std::string where;
};

template <typename SM>
class SimpleModelField : public TextField {
public:
  using TextField::TextField;
  SimpleModelField(const std::string& label, ImGuiInputTextFlags = 0, TFFlags = 0, const std::string& column = "");
  SM& Render();
  void Validate() override;
  void Feedback() override;
  SM GetFromDb();

private:
  SM model;
  Popup<SM> popup;
  std::string column;
};

// Currently it works only with <Color, DeviceField> which is suboptimal
template <typename SM, typename R>
class RelationalField : public TextField {
public:
  using TextField::TextField;
  SM& Render(R& related);
  void Validate() override;
  void Feedback() override;
  SM GetFromDb();

private:
  SM model;
  Popup<SM> popup;
};
