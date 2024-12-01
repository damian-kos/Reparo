#include "text_fields.h"
#include "imgui.h"
#include <iostream>
#include "RoLocalization.h"
#include <string>
#include <vector>
#include "models/customer.h"
#include "models/device.h"
#include "models/supplier.h"


template <typename T>
ValidatorFlags Validator::DatabaseChk(const std::string& table, const std::string& condition) {
  std::cout << "DatabaseChk is run" << std::endl;
  auto record = Database::Select<T>().From(table).Where(condition).One();
  std::cout << Database::Select<T>().From(table).Where(condition).GetSql() << std::endl;
  if(record.id > 0)
    return ValidatorFlags_IsDuplicate;
  return ValidatorFlags_Pass;
}

ValidatorFlags Validator::StrLen(const std::string& buffer, const int& min_len){
  if (buffer.size() < min_len)
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

template <typename T>
bool Popup<T>::OnTextInput(std::string& buffer, const std::vector<T>& data) {
    float x = ImGui::GetWindowContentRegionMax().x;
    int _selected = -1;
    bool state = false;
    bool is_input_active = ImGui::IsItemActive();
    bool is_input_activated = ImGui::IsItemActivated();
    if (is_input_activated) {
      ImGui::OpenPopup("Popup");
      selected = -1;
    }

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
      if (ImGui::BeginPopup("Popup", ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_ChildWindow))
      {

        ImVec2 scrolling_child_size = ImVec2(x - 30, ImGui::GetFrameHeightWithSpacing() * 3 + 30);
        ImGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
        ImGui::HelpMarker("Click auto fill with selection.");
        for (auto& _record : data) {
          std::string label = _record.ToString();
          if (ImGui::Selectable(label.c_str())) {
            _selected = _record.id;
            record = _record;
            selected = _record.id;
            state = true;
            ImGui::CloseCurrentPopup();           
          }
      }
      if (!is_input_active && !ImGui::IsWindowFocused()) {
        std::cout << "Close current popup #2" << std::endl;
        ImGui::CloseCurrentPopup();
      }
      float scroll_x = ImGui::GetScrollX();
      float scroll_max_x = ImGui::GetScrollMaxX();
      ImGui::EndChild();
      ImGui::EndPopup();
    }
      return state;
  }

TextField::TextField() { }

TextField::TextField(const std::string& label, ImGuiInputTextFlags flags, TFFlags ro_flags)
  : label(label), flags(flags), ro_flags(ro_flags) { }

int TextField::Render() {
  Field();

  if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
    Validate();
  }
  return -1;
}

void TextField::Field(){
  ImGui::BeginValid(has_error_with_content);
  ImGui::InputTextWithHint(("##" + label).c_str(), (label + "...").c_str(), &buffer, flags);
  ImGui::EndValid(has_error_with_content);
}

void TextField::Feedback() { 
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("%s too short", label.c_str()); ImGui::SameLine();
  }
}

void TextField::Validate() { 
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

const std::string& TextField::Get() const{
  return buffer;
}

const void TextField::Clear() {
  buffer.clear();
}

void TextField::FillBuffer(const std::string& fill) {
  buffer = fill;
}

bool PhoneField::Render() {
  static std::vector<Customer> vec;
  static int autofill = -1;
  bool state = false;
  Field();

  if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
    Validate();
    vec.clear();
    vec = Database::Select<Customer>()
      .From("customers")
      .Where("phone")
      .Like(buffer)
      .All();
  }

  ImGui::PushID(label.c_str());
  Customer temp;
  if (ro_flags & TFFlags_HasPopup) {
    if (popup.OnTextInput(buffer, vec)) {
      buffer = popup.record.phone;
      customer = GetFromDb();
      Validate();
      state = true;
    }
  }
  ImGui::PopID();
  return state;
}

void PhoneField::Validate() {
  err_flags = ValidatorFlags_Pass;
  if (!(ro_flags & TFFlags_AllowDbPresence))
    err_flags |= Validator::DatabaseChk<Customer>("customers", "phone = " + buffer); // Edit with corresponding table once data in db exists
  err_flags |= Validator::StrLen(buffer, 7);
  error = err_flags & (ValidatorFlags_StrLen | ValidatorFlags_IsDuplicate);
  has_error_with_content = error && buffer.size() > 0;
}

void PhoneField::Feedback() {
  // When input field is empty there is no point of giving feedback.
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("Phone too short"); ImGui::SameLine();
  }
  if (err_flags & ValidatorFlags_IsDuplicate) {
    ImGui::Text("Phone already exists"); ImGui::SameLine();
  }
}

Customer PhoneField::GetFromDb() {
  return Database::Get().Customer_<std::string>(buffer);
}

int NameField::Render() {
  Field();

  if (ImGui::IsItemEdited()) {
    Validate();
  }

  return -1;
}

void NameField::Validate() {
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

void NameField::Feedback() {
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("Name too short"); ImGui::SameLine();
  }
}

int SurnameField::Render() {
  Field();

  if (ImGui::IsItemEdited()) {
    Validate();
  }

  return -1;
}

void SurnameField::Validate() {
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

void SurnameField::Feedback() {
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("Surname too short"); ImGui::SameLine();
  }
}

int EmailField::Render() {
  Field();

  if (ImGui::IsItemEdited()) {
    Validate();
  }
  return -1;
}

void EmailField::Validate() {
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::IsEmail(buffer);
  error = err_flags & ValidatorFlags_NotEmail;
  has_error_with_content = error && buffer.size() > 0;
}

void EmailField::Feedback() {
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_NotEmail) {
    ImGui::Text("Invalid email"); ImGui::SameLine();
  }
}

Device DeviceField::Render() {
  static std::vector<Device> vec;
  Field();
 
  if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
    device = Device();
    Validate();
    vec.clear();
    vec = Database::Select<Device>(" devices.*, dt.type, b.brand ")
      .From("devices")
      .LeftJoin(" device_types dt ON devices.type_id = dt.id")
      .LeftJoin(" brands b ON devices.brand_id = b.id ")
      .Where("model")
      .Like(buffer)
      .All();
  }
  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HasPopup) {
    if (popup.OnTextInput(buffer, vec)) {
      buffer = popup.record.name;
      device = GetFromDb();
      Validate();
    }
  }
  ImGui::PopID();
  return device;
}

void DeviceField::Validate() {
  err_flags = ValidatorFlags_Pass;
  // Database check can be moved after length check to avoid unecessary checks
  std::string buf = "'" + buffer + "'";
  err_flags |= Validator::DatabaseChk<Device>("devices", "model = " + buf); // Edit with corresponding table once data in db exists
  err_flags |= Validator::StrLen(buffer, 3);
  if (!(ro_flags & TFFlags_AllowDbPresence))
    error = err_flags & (ValidatorFlags_StrLen | ValidatorFlags_IsDuplicate);
  else
    error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

void DeviceField::Feedback() {
  // When input field is empty there is no point of giving feedback.
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text(_("Device model too short")); ImGui::SameLine();
  }
  if (err_flags & ValidatorFlags_IsDuplicate) {
    ImGui::Text(_("Device already exists")); ImGui::SameLine();
  }
}

Device DeviceField::GetFromDb() {
  return Database::Get().Device_(buffer);
}

bool DeviceField::IsInDb() {
  return (err_flags & ValidatorFlags_IsDuplicate) || error ;
}

template<typename SM>
SM& SimpleModelField<SM>::Render() {
  static std::vector<SM> vec;
  Field();

  if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
    Validate();
    vec.clear();
    vec = Database::Select<SM>().From().Where(model.column).Like(buffer).All();
  }
  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HasPopup) {
    if (popup.OnTextInput(buffer, vec)) {
      buffer = popup.record.name;
      model = GetFromDb();
      Validate();
    }
  }
  ImGui::PopID();
  return model;
}

template<typename SM>
void SimpleModelField<SM>::Validate() {
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

template<typename SM>
void SimpleModelField<SM>::Feedback() {
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("%s too short", label.c_str()); ImGui::SameLine();
  }
}

template<typename SM>
SM SimpleModelField<SM>::GetFromDb() {
  if (buffer.empty()) {
    SM model;
    model.id = -1;
    model.name = buffer;
    return model; // Return a default model with invalid id and buffer as name
  }
  return Database::Get().SimpleModel_<std::string, SM>(buffer);
}

template struct SimpleModelField<Brand>;
template struct SimpleModelField<RepairState>;
template struct SimpleModelField<RepairCategory>;
template struct SimpleModelField<PaymentMethod>;
template struct SimpleModelField<DeviceType>;
template struct SimpleModelField<Color>;
template struct SimpleModelField<Supplier>;

// Currently it works only with <Color, DeviceField> which is suboptimal
template<typename SM, typename R>
SM& RelationalField<SM, R>::Render(R& related) {
  static std::vector<SM> vec;
  static int curr_id = -1;
  Field();
  if (ImGui::IsItemActivated()) {
    curr_id = related.GetFromDb().id;
    vec.clear();
    if (curr_id > 0) {
      std::string _id_str = std::to_string(curr_id);
      // Smells, we are also using it attributes.h
      vec = Database::Select<SM>("id, color") // Smells we are 
        .From("colors")
        .InnerJoin("model_colors mc ON colors.id = mc.color_id ")
        .Where("mc.model_id = " + _id_str).All();
    }
    if (vec.empty()) {
      vec = Database::Select<SM>().From().All();
    }
  }
  if (ImGui::IsItemEdited()) {
    Validate();
  }
  ImGui::PushID(label.c_str());
  if (ro_flags & TFFlags_HasPopup) {
    if (popup.OnTextInput(buffer, vec)) {
      buffer = popup.record.name;
      model = GetFromDb();
      Validate();
    }
  }
  ImGui::PopID();
  return model;
}

template<typename SM, typename R>
void RelationalField<SM, R>::Validate() {
  err_flags = ValidatorFlags_Pass;  // Reset flags
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & ValidatorFlags_StrLen;
  has_error_with_content = error && buffer.size() > 0;
}

template<typename SM, typename R>
void RelationalField<SM, R>::Feedback() {
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("%s too short", label.c_str()); ImGui::SameLine();
  }
}

template<typename SM, typename R>
SM RelationalField<SM, R>::GetFromDb() {
  if (buffer.empty()) { return SM(); }
  return Database::Get().SimpleModel_<std::string, SM>(buffer);
}

template struct RelationalField<Color, DeviceField>;

bool OwnSKUField::Render(){
  static std::vector<Part> vec;
  static int autofill = -1;
  bool state = false;
  Field();

  if (ImGui::IsItemEdited() || ImGui::IsItemActivated()) {
    Validate();
    vec.clear();
    vec = Database::Select<Part>()
      .From("parts")
      .Where("own_sku")
      .Like(buffer)
      .All();
  }

  ImGui::PushID(label.c_str());
  Part temp;
  if (ro_flags & TFFlags_HasPopup) {
    if (popup.OnTextInput(buffer, vec)) {
      buffer = popup.record.own_sku;
      // Currently we dont need to get the whole Part objects from database
      //part = GetFromDb();
      Validate();
      state = true;
    }
  }
  ImGui::PopID();
  return state;
}

void OwnSKUField::Validate() {
  err_flags = ValidatorFlags_Pass;
  std::string buf = "'" + buffer + "'";
  if (!(ro_flags & TFFlags_AllowDbPresence))
    err_flags |= Validator::DatabaseChk<Part>("parts", "own_sku = " + buf); 
  err_flags |= Validator::StrLen(buffer, 3);
  error = err_flags & (ValidatorFlags_StrLen | ValidatorFlags_IsDuplicate);
  has_error_with_content = error && buffer.size() > 0;
}

void OwnSKUField::Feedback() {
  // When input field is empty there is no point of giving feedback.
  if (!has_error_with_content) { return; }
  if (err_flags & ValidatorFlags_StrLen) {
    ImGui::Text("SKU too short"); ImGui::SameLine();
  }
  if (err_flags & ValidatorFlags_IsDuplicate) {
    ImGui::Text("Own SKU already exists"); ImGui::SameLine();
  }
}

Part OwnSKUField::GetFromDb() {
  // Currently we dont need to get the whole Part objects from database
  return Part();
}
