#include "reparo_windows.h"
#include "imgui.h"
#include "imgui_ex.h"
#include "models/customer.h"
#include "models/simple_models.h"
#include "database.h"
#include "models/repair.h"
#include "models/parts.h"

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
    Customer customer = CreateCustomer();
    ModalConfig config;
    config.Title(_("Insert Customer?"))
      .Msg(_("Are you sure?"));

    CustomerModal modal(customer, config);
    ModalManager::SetModal(modal);
  }
  ImGui::EndDisabled();

}

void CustomerWin::FillBuffersByPhone(Customer&  _customer) {
  if (_customer.id > 0) {
    name.FillBuffer(_customer.name);
    surname.FillBuffer(_customer.surname);
    email.FillBuffer(_customer.email);
    for (int i = 0; i < billing_address.size(); ++i) {
      billing_address[i].FillBuffer(_customer.billing_addresses.Get().Lines()[i]);
      ship_address[i].FillBuffer(_customer.ship_addresses.Get().Lines()[i]);
    }
    // After filling buffer, need to revalidate fields.
    name.Validate();
    surname.Validate();
    email.Validate();
  }
}

void CustomerWin::InputFields() {
  // Needs to be cleaned up we are calling GetFromDb() too many times
  if(phone.Render())
    FillBuffersByPhone(phone.customer);
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

/// <summary>
/// Creates custoemr object from filled fields.
/// </summary>
/// <returns></returns>
Customer CustomerWin::CreateCustomer() {
  Customer customer;
  customer.phone = phone.Get();
  customer.name = name.Get();
  customer.surname = surname.Get();
  customer.email = email.Get();
  customer.billing_addresses.SetLines(billing_address);
  customer.ship_addresses.SetLines(ship_address);
  return customer;
}

/// <summary>
/// Gets customer from CustomerWin. If customer is present in Database, get Customer with all its data from there, otherwise
/// return the customer created with current input fields buffers.
/// </summary>
/// <returns></returns>
Customer CustomerWin::GetCustomer() {
  Customer temp = phone.GetFromDb();
  Customer customer = CreateCustomer();
  if (temp.id > 0) {
    customer.id = temp.id;
    int billing_id = temp.billing_addresses.Get().ID();
    customer.billing_addresses.SetID(billing_id);
    int ship_id = temp.ship_addresses.Get().ID();
    customer.ship_addresses.SetID(ship_id);
  }
  return customer;
  
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
  , price_can_be_zero(true)
  , device(_("Model"), 0, TFFlags_HasPopup | TFFlags_AllowDbPresence)
  , category(_("Category"), 0, TFFlags_HasPopup)
  , color(_("Color"), 0, TFFlags_HasPopup)
  , sn_imei(_("Serial / IMEI"))
  , vis_note(_("Notes for customer"))
  , hid_note(_("Notes hidden from customer"))
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
    repair.device = CreateDevice();
    repair.category = category.GetFromDb();
    repair.color = color.GetFromDb();
    repair.sn_imei = sn_imei.Get();
    repair.vis_note = vis_note.Get();
    repair.hid_note = hid_note.Get();
    repair.price = price;
    repair.repair_state = Database::Get().SimpleModel_<int, RepairState>(2);
    repair.cust_device_id = device.IsInDb() ? -1 : 1;

    ModalConfig config;
    config.Title(_("Insert new repair?"));
    RepairModal modal(repair, config);
    ModalManager::SetModal(modal);

  }
  ImGui::EndDisabled();
}

void RepairWin::RepairValidated() {
  error = customer_section.error ||  device_section_error || notes_section_error || price_section_error;
}

Device RepairWin::CreateDevice() {
  Device _device = device.GetFromDb();
  if (_device.id > 0) {
    return _device;
  }
  _device.name = device.Get();
  Color _device_color =  color.GetFromDb();;
  if (_device_color.id < 0) {
    _device_color.name = color.Get();
  }
  _device.colors.push_back(_device_color);
  return _device;
}

PartsWin::PartsWin()
: supplier(_("Supplier"), 0, TFFlags_HasPopup)
, own_sku_field(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError)
, name_field(_("Item's name"), 0, TFFlags_HasPopup, "name")
, color(_("Color"), 0, TFFlags_HasPopup)
, qualities(_("Quality"), 0, TFFlags_HasPopup)
, category(_("Category"), 0, TFFlags_HasPopup)
, location(_("Location"), 0, TFFlags_HasPopup, "location", "parts", "location")
, device_filter(_("Device's model"), 0, TFFlags_HasPopup, "model", "devices", "model")
, brand_filter(_("Brand"), 0, TFFlags_HasPopup)
, device_type_filter(_("Device type"), 0, TFFlags_HasPopup)

{ }

void PartsWin::Render() {
  // Currently in ModalPopup, possible change later
  // Also this content needs to be put in ImGui's table
  if (ImGui::Button(_("Parts"))) {
    open = true;
    LoadDevices();
    ImGui::OpenPopup(_("Insert part"));
  }
  if (ImGui::BeginPopupModal(_("Insert part"), &open)) {
    if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_SizingStretchProp)) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      supplier.Render();
      own_sku_field.Render();
      name_field.Render();
      PriceSection("buy", buy_price);
      PriceSection("sell", sell_price);
      QuantitySection();

      ImGui::TableNextColumn();

      color.Render();
      qualities.Render();
      category.Render();
      location.Render();
      CompatibleEntriesBox();
      Submit();
      //own_sku_field.FeedbackEx( "", "", "", "", "" );
      name_field.Feedback();

      ImGui::EndTable();
    }

    Filters();
    CompatibleTablePicker();
    ImGui::EndPopup();
  }
}

void PartsWin::PriceSection(const std::string& _action, Price& _price) {
  std::string separator;
  std::string label_1;
  std::string label_2;
  std::string label_3;
  if (_action == "buy") {
    separator = _("BUY PRICING");
    label_1 = _("Buy price (inc. VAT)");
    label_2 = _("Ex.VAT buy price");
    label_3 = _("Price ex.VAT: % .2f");
  }
  if (_action == "sell") {
    separator = _("SELL PRICING");
    label_1 = _("Sell price (inc. VAT)");
    label_2 = _("Ex.VAT sell price");
    label_3 = _("Price ex.VAT: % .2f");
  }
  ImGui::PushID(_action.c_str());
  ImGui::BeginGroup();
  ImGui::PushItemWidth(100);
  ImGui::SeparatorText(separator.c_str());
  ImGui::InputDouble(label_1.c_str(), &_price.amount, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::Checkbox(label_2.c_str(), &_price.ex_vat);
  ImGui::EndGroup();

  ImGui::BeginGroup();
  ImGui::BeginDisabled(!_price.ex_vat);
  ImGui::PushItemWidth(100);
  ImGui::InputDouble(_("VAT rate"), &_price.vat_rate, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::EndDisabled();
  ImGui::Text((label_3 + " %.2f").c_str(), _price.ExVat());
  ImGui::EndGroup();
  ImGui::PopID();
}

void PartsWin::QuantitySection() {
  ImGui::InputInt(_("Quantity"), &quantity);
}

void PartsWin::CompatibleTablePicker() {
  RoTable::TableWithDevices(devices, cmptble_devices, cmptble_aliases);
}

void PartsWin::CompatibleEntriesBox() {
  ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 3 + 30);
  ImGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
  ImGui::HelpMarker("Click to remove device from list of compatible devices.");
  float window = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  float last_btn = ImGui::GetCursorPosX(); // Initialize with the starting cursor position of the window

  ImGui::SeparatorText(_("Devices"));
  ListEntriesInBox<Device>(last_btn, window, cmptble_devices);

  ImGui::SeparatorText(_("Aliases"));
  ListEntriesInBox<Alias>(last_btn, window, cmptble_aliases);

  ImGui::EndChild();
}

template <typename T>
void PartsWin::ListEntriesInBox(float& _last_btn, float _window, std::unordered_map<int, T>& _entries) {
  if (_entries.empty())
    return;

  for (auto it = _entries.begin(); it != _entries.end(); ) {
   // bool should_continue = false;

    if (ImGui::Button(_entries[it->first].name.c_str())) {
      it = _entries.erase(it);
      continue;
    }

    auto next_it = std::next(it, 1);
    if (next_it != _entries.end()) {
      _last_btn = ImGui::GetItemRectMax().x + (ImGui::CalcTextSize(next_it->second.name.c_str()).x * 1.2);

      if (_last_btn < _window)
        ImGui::SameLine();
    }

    ++it;
  }
}

void PartsWin::Submit() {
  if (ImGui::Button(_("Submit"))) {

  }
}

void PartsWin::Filters() {
  ImGui::NewLine();
  ImGui::SeparatorText(_("Compatible devices or devices' versions"));
  device_filter.Render();
  brand_filter.Render();
  device_type_filter.Render();
  if(ImGui::Button(_("Search"))) {
    LoadDevices();
  }
}

void PartsWin::LoadDevices() {
    devices.clear();

    std::string brand_id_str = "";
    int brand_id = brand_filter.GetFromDb().id;
    if (brand_id > 0)
      brand_id_str = "brand_id = " + std::to_string(brand_id);

    std::string type_id_str = "";
    int type = device_type_filter.GetFromDb().id;
    if (type > 0)
      type_id_str = "type_id = " + std::to_string(type);

    devices = Database::Select<Device>().From().Where("model").Like(device_filter.Get())
      .And(brand_id_str)
      .And(type_id_str)
      .All();

  for (auto& device : devices) {
    std::string _id_str = std::to_string(device.id);
    device.aliases = (Database::Select<Alias>().From().Where("model_id = " + _id_str).All());
  }

}
