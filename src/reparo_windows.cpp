#include "reparo_windows.h"
#include "imgui.h"
#include "imgui_ex.h"
#include "models/customer.h"
#include "models/simple_models.h"
#include "database.h"
#include "models/repair.h"


CustomerWin::CustomerWin()
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup) { 
  Init();
}

CustomerWin::CustomerWin(TFFlags phoneFlags)
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, phoneFlags) {
  Init();
}

void CustomerWin::Init() {
  name = NameField(_("Name"));
  surname = SurnameField(_("Surname"));
  email = EmailField(_("Email"));

  billing_address.resize(5);
  ship_address.resize(5);

  // Initialize address fields
  for (int i = 0; i < 5; i++) {
    std::string label = _("Billing Address Line ") + std::to_string(i + 1);
    billing_address[i] = TextField(label);

    label = _("Shipping Address Line ") + std::to_string(i + 1);
    ship_address[i] = TextField(label);
  }
}


void CustomerWin::Render() {
  ImGui::Begin(_("Insert Customer"), &open);
  ImGui::SeparatorColor(_("CUSTOMER"), error);
  Feedback();

  InputFields();
  Addresses();

  Debug();

  Submit();

  ImGui::End();
}

void CustomerWin::Debug(){
#ifdef DEBUG
  ImGui::Text("%s", phone.error ? "true" : "false");
  ImGui::Text("%s", name.error ? "true" : "false");
  ImGui::Text("%s", surname.error ? "true" : "false");
  ImGui::Text("%s", email.error ? "true" : "false");
  ImGui::Text("%s", error ? "true" : "false");
#endif // DEBUG
}

void CustomerWin::Feedback() {
  FieldsValidate();
  phone.Feedback();
  name.Feedback();
  surname.Feedback();
  email.Feedback();
  ImGui::NewLine(); // so we don't need to worry about calling ImGui::SameLine in Feedback methods of TextFields();
}

void CustomerWin::Submit() {
  ImGui::BeginDisabled(error);
  if (ImGui::Button(_("Submit Customer Details"))) {
    Customer customer;
    customer.phone = phone.Get();
    customer.name = name.Get();
    customer.surname = surname.Get();
    customer.email = email.Get();
    customer.billing_addresses.SetLines(billing_address);
    customer.ship_addresses.SetLines(ship_address);

    BuildCustomerModal build;
    build.Title(_("Insert Customer?"))
      .Message(_("Are you sure?")).SetCustomer(customer);

    CustomerModal modal(build.Build());
    ModalManager::SetModal(modal);
  }
  ImGui::EndDisabled();

}

void CustomerWin::FillBuffersByPhone(Customer&  _customer) {
  if (_customer.id > 0) {
    name.FillBuffer(_customer.name);
    surname.FillBuffer(_customer.surname);
    email.FillBuffer(_customer.email);
    // After filling buffer, need to revalidate fields.
    name.Validate();
    surname.Validate();
    email.Validate();
  }
}

void CustomerWin::InputFields() {
  Customer _customer;
  _customer = phone.Render();
  FillBuffersByPhone(_customer);
  name.Render();
  surname.Render();
  email.Render();
}

void CustomerWin::Addresses() {
  if (ImGui::CollapsingHeader("Addresses", ImGuiTreeNodeFlags_None)) {
    RoTable::AddressesInputs(billing_address, ship_address);
  }
}

void CustomerWin::FieldsValidate() {
  error = phone.error || name.error || surname.error || email.error;
}

Customer CustomerWin::GetCustomer() {
  return phone.GetFromDb();
}

DeviceWin::DeviceWin() 
  : name(_("Model"), 0, TFFlags_HasPopup)
  , brand_combo(_("Choose brand"))
  , type_combo(_("Choose type"))
{
}

void DeviceWin::Render() { 
// It is suppposed to be a modal window so change ImGui::Begin() to BeginPopupModal later on.
  if (ImGui::Button(_("Device"))) {
    ImGui::OpenPopup(_("Insert new device"));
    open = true;
  }
  if (ImGui::BeginPopupModal(_("Insert new device"), &open)) {

    static ImGuiTableFlags _flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersInnerV |
      ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable;

    static std::vector<std::string> _col_names = { _("Model"), _("Type"), _("Brand"), _("Colors"), _("Aliases") };

    if (ImGui::BeginTable("Devices", 5, _flags)) {
      for (auto& _name : _col_names)
        ImGui::TableSetupColumn(_name.c_str());

      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      ImGui::TableNextColumn();
      DeviceName();

      ImGui::TableNextColumn();
      type_combo.Render();

      ImGui::TableNextColumn();
      brand_combo.Render();

      ImGui::TableNextColumn();
      colors.Render();

      ImGui::TableNextColumn();
      aliases.Render();

      ImGui::EndTable();
    }

    if (ImGui::Button(_("Save"))) {
      Device _device;
      _device.name = name.Get();
      _device.brand = brand_combo.Get();
      _device.type = type_combo.Get();
      _device.colors = colors.Get();
      _device.aliases = aliases.Get();
      Database::Insert().Device_(_device);

    }
    ImGui::EndPopup();
  }
}

void DeviceWin::DeviceName() {
  static std::string _model;
  std::string _label = _model.empty() ? _("Add model") : _model;
  bool _focus = false;
  _focus = ImGui::Button(_label.c_str());
  if (ImGui::BeginPopupContextItem("Name edit", ImGuiPopupFlags_MouseButtonLeft)) {
    if (_focus) ImGui::SetKeyboardFocusHere();
    static Device autofill;
    autofill = name.Render();
    ImGui::SameLine();
    if (ImGui::Button(_("Save"))) {
      FillDeviceByName(autofill);
      _model = name.Get();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button(_("Cancel"))) {
      name.Clear();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DeviceWin::FillDeviceByName(Device& autofill) {
  std::cout << autofill.ToString() << std::endl;
  type_combo.SetLabel(autofill.type.name);
  brand_combo.SetLabel(autofill.brand.name);
  colors = Attributes<Color>(autofill.id);
  aliases = Attributes<Alias>(autofill.id);
}

RepairWin::RepairWin()
  : customer_section(TFFlags_HasPopup | TFFlags_AllowDbPresence)
  , device(_("Model"), 0, TFFlags_HasPopup | TFFlags_AllowDbPresence)
  , category(_("Category"), 0, TFFlags_HasPopup)
  , color(_("Color"), 0, TFFlags_HasPopup)
  , sn_imei(_("Serial / IMEI"))
  , vis_note(_("Notes for customer"))
  , hid_note(_("Notes hidden from customer"))
  , price_can_be_zero(true)
{}

void RepairWin::Render() {
  ImGui::Begin(_("Repair"));
  FieldsValidate();
  RepairValidated();
  CustomerSection();
  DeviceSection();
  NotesSection();
  PriceSection();
  Submit();
  ImGui::End();
}

void RepairWin::CustomerSection() {
  ImGui::SeparatorColor(_("CUSTOMER"), customer_section.error);
  customer_section.Feedback();
  customer_section.InputFields();
  customer_section.Debug();
  customer_section.Addresses();
}

void RepairWin::DeviceSection() {
  ImGui::SeparatorColor(_("DEVICE"), device_section_error);
  DeviceFeedback();
  ImGui::NewLine();
  device.Render();
  category.Render();
  color.Render(device);
  sn_imei.Render();
  if (!device.IsInDb())
    ImGui::Text("Custom device");
  else
    ImGui::Text("");
}

void RepairWin::NotesSection() {
  ImGui::SeparatorColor(_("NOTES"), notes_section_error);
  NotesFeedback();
  ImGui::NewLine();
  vis_note.Render();
  hid_note.Render();
}

void RepairWin::FieldsValidate() {
  device_section_error = device.error || category.error || color.error || sn_imei.error;
  notes_section_error = vis_note.error || hid_note.error;
  if (price_can_be_zero) {
    price_section_error = price < 0; // Error only if price is negative
  }
  else {
    price_section_error = price <= 0; // Error if price is zero or negative
  }
}

void RepairWin::DeviceFeedback() {
  device.Feedback();
  category.Feedback();
  color.Feedback();
  sn_imei.Feedback();
}

void RepairWin::NotesFeedback() {
  vis_note.Feedback();
  hid_note.Feedback();
}

void RepairWin::PriceSection(){
  ImGui::SeparatorColor(_("PRICE"), price_section_error);
  PriceFeedback(); 
  ImGui::InputDouble("##Price", &price, 0.10, 1.0, "%.2f");
}

void RepairWin::PriceFeedback() {
  if (price_section_error)
    ImGui::Text("Price needs to be more than 0");
  else
    ImGui::NewLine();
}

void RepairWin::Submit() {
  ImGui::SeparatorColor(_("SUBMIT"), error);
  ImGui::BeginDisabled(error);
  if (ImGui::Button(_("Submit Repair"))) {
    Repair repair;
    repair.customer = customer_section.GetCustomer();
    // We can change database query below if we needed to get brand and type of the device
    repair.device = device.GetFromDb();
    repair.category = category.GetFromDb();
    repair.color = color.GetFromDb();
    repair.sn_imei = sn_imei.Get();
    repair.vis_note = vis_note.Get();
    repair.hid_note = hid_note.Get();
    repair.price = price;
    repair.repair_state = RepairState(2);
    
    // Create repair object;
  }
  ImGui::EndDisabled();
}

void RepairWin::RepairValidated() {
  error = customer_section.error ||  device_section_error || notes_section_error || price_section_error;
}

