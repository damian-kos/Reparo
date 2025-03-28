#include "reparo_windows.h"
#include "imgui.h"
#include "imgui_ex.h"
#include "models/customer.h"
#include "models/simple_models.h"
#include "database.h"
#include "models/repair.h"
#include "models/parts.h"
#include "modal.h"
#include "tables.h"
#include <map>
#include "filters.h"

CustomerWin::CustomerWin()
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup | TFFlags_EmptyIsError) { 
  Init();
}

CustomerWin::CustomerWin(TFFlags phoneFlags)
  : phone(_("Phone"), ImGuiInputTextFlags_CharsDecimal, phoneFlags) {
  Init();
}

CustomerWin::CustomerWin(Customer& _customer) {
  Init();
  phone = PhoneField(_("Phone3"), ImGuiInputTextFlags_CharsDecimal, TFFlags_HasPopup | TFFlags_EmptyIsError);
  state = WindowState_Update;
  previous_customer = _customer;
  FillBuffersByPhone(_customer);
}

CustomerWin::~CustomerWin() { 
  std::cout << "Customer window destroyed" << std::endl;
}

void CustomerWin::Init() {
  std::cout << "Customer window initialized" << std::endl;
  open = true;
  name = NameField(_("Name"), ImGuiBackendFlags_None, TFFlags_EmptyIsError);
  surname = SurnameField(_("Surname"), ImGuiBackendFlags_None, TFFlags_EmptyIsError);
  email = EmailField(_("Email"), ImGuiBackendFlags_None, TFFlags_EmptyIsError);

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
  RenderInsertState();
  RenderUpdateState();
}

void CustomerWin::Debug(){
#ifdef DEBUG
  ImGui::Text("Phone error:   %s", phone.error ? "true" : "false");
  ImGui::Text("Name error:    %s", name.error ? "true" : "false");
  ImGui::Text("Surname error: %s", surname.error ? "true" : "false");
  ImGui::Text("Email error:   %s", email.error ? "true" : "false");
  ImGui::Text("Error:         %s", error ? "true" : "false");
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
    customer.InsertModal();

  }
  ImGui::EndDisabled();

}

void CustomerWin::FillBuffersByPhone(Customer&  _customer) {
  if (_customer.id > 0) {
    phone.FillBuffer(_customer.phone);
    name.FillBuffer(_customer.name);
    surname.FillBuffer(_customer.surname);
    email.FillBuffer(_customer.email);
    _customer.LoadAddress();
    if (_customer.billing_addresses.Get().Lines().size() > 0)
      for (int i = 0; i < billing_address.size(); ++i) {
        billing_address[i].FillBuffer(_customer.billing_addresses.Get().Lines()[i]);
      }
    if (_customer.ship_addresses.Get().Lines().size() > 0)
      for (int i = 0; i < ship_address.size(); ++i) {
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
  if (phone.Render())
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
  if (state == WindowState_Update) {
    if (previous_customer.Equals(CreateCustomer(), true)) {
      error = true;
    }
    else {
      if (previous_customer.phone == phone.Get()) {
        error = name.error || surname.error || email.error;
      }
      else {
        error = phone.error || name.error || surname.error || email.error;
      }
    }
  }
  else {
    error = phone.error || name.error || surname.error || email.error;
  }
}

/// <summary>
/// Creates custoemr object from filled fields.
/// </summary>
/// <returns></returns>
Customer CustomerWin::CreateCustomer() {
  Customer _customer;
  _customer.phone = phone.Get();
  _customer.name = name.Get();
  _customer.surname = surname.Get();
  _customer.email = email.Get();
  _customer.billing_addresses.SetLines(billing_address);
  _customer.ship_addresses.SetLines(ship_address);
  return _customer;
}

/// <summary>
/// Gets customer from CustomerWin. If customer is present in Database, get Customer with all its data from there, otherwise
/// return the customer created with current input fields buffers.
/// </summary>
/// <returns></returns>
Customer CustomerWin::GetEntity() {
  Customer customer = CreateCustomer();
  Customer temp = phone.GetFromDb();
  if (temp.id > 0) {
    customer.id = temp.id;
    int billing_id = temp.billing_addresses.Get().ID();
    customer.billing_addresses.SetID(billing_id);
    int ship_id = temp.ship_addresses.Get().ID();
    customer.ship_addresses.SetID(ship_id);
  }
  return customer;
}

bool CustomerWin::GetError() const {
  return error;
}

Customer& CustomerWin::GetPrevious() {
  return previous_customer;
}

void CustomerWin::RenderInsertState() {
  if (state == WindowState_Insert) {
    ImGui::Begin(_("Insert Customer"), &open);
    ImGui::SeparatorColor(_("CUSTOMER"), error);
    Feedback();

    InputFields();
    Addresses();

    Debug();

    Submit();

    ImGui::End();
  }
}

void CustomerWin::RenderUpdateState() {
  if (state == WindowState_Update) {
    ImGui::SeparatorColor(_("CUSTOMER"), error);
    Feedback();

    InputFields();
    Addresses();

    StackModal::RenderModal();
    Debug();
  }
} 

DeviceWin::DeviceWin() {
  Init();
}

DeviceWin::DeviceWin(Device _device) {
  state = WindowState_Update;
  previous_device = _device;
  name.FillBuffer(_device.name);
  brand_combo = RoCombo<Brand>(_device.brand);
  type_combo = RoCombo<DeviceType>(_device.type);
  colors = Attributes<Color>(_device.id);
  aliases = Attributes<Alias>(_device.id);
}

DeviceWin::DeviceWin(CustomDevice _custom) {
  Init();
  previous_device = _custom; // so we can assign 'name'
  state = WindowState_Insert;
  name.FillBuffer(_custom.name);
  std::string _where = "cd.model = '" + _custom.name + "'";
  auto _device_colors = Database::Select<Color>("DISTINCT c.id, cd.color")
    .From("custom_devices cd")
    .LeftJoin("colors c")
    .On("c.color = cd.color")
    .Where(_where)
    .All();
  for (auto& color : _device_colors) 
    colors.Insert(color.id, color.name);
}

DeviceWin::~DeviceWin() {
  std::cout << "Device window destroyed" << std::endl;
}

void DeviceWin::Init() {
  open = true;
  name = DeviceField(_("Model"), 0);
  brand_combo = RoCombo<Brand>(_("Choose brand"));
  type_combo = RoCombo<DeviceType>(_("Choose type"));
}

void DeviceWin::Render() { 
  RenderInsertState();
  RenderUpdateState();
}

void DeviceWin::DeviceName() {
  //std::string _model = name.Get();
  std::string _label = name.Get().empty() ? _("Add model") : name.Get();
  bool _focus = false;
  _focus = ImGui::Button(_label.c_str());
  if (ImGui::BeginPopupContextItem("Name edit", ImGuiPopupFlags_MouseButtonLeft)) {
    
    if (_focus) { 
      ImGui::SetKeyboardFocusHere(); 
    }
    Device autofill;
    autofill = name.Render();
    ImGui::SameLine();
    if (ImGui::Button(_("Save"))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button(_("Cancel"))) {
      name.FillBuffer(previous_device.name);
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

Device DeviceWin::CreateDevice() {
  Device _device;
  _device.name = name.Get();
  _device.brand = brand_combo.Get();
  _device.type = type_combo.Get();
  _device.colors = colors.Get();
  _device.aliases = aliases.Get();
  return _device;
}

void DeviceWin::RenderSharedState() {
  static ImGuiTableFlags _flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp |
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Resizable;

  static std::vector<std::string> _col_names = { _("Model"), _("Type"), _("Brand"), _("Colors"), _("Aliases") };

  if (ImGui::BeginTable("Devices", 5, _flags)) {
    for (auto& _name : _col_names)
      ImGui::TableSetupColumn(_name.c_str());

    ImGui::TableHeadersRow();
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    DeviceName();

    ImGui::TableNextColumn();
    type_combo.RenderFromBtn();

    ImGui::TableNextColumn();
    brand_combo.RenderFromBtn();

    ImGui::TableNextColumn();
    colors.Render();

    ImGui::TableNextColumn();
    aliases.Render();

    ImGui::EndTable();
  }
}

bool DeviceWin::Submit() {
  return ImGui::Button(_("Save"));
}

void DeviceWin::RenderInsertState() {
  if (state == WindowState_Insert) {
      RenderSharedState();
      SubmitInsert();
  }
}

void DeviceWin::SubmitInsert() {
  if (Submit()) {
    Device _device = CreateDevice();
    Database::Insert().Device_(_device);
  }
}

void DeviceWin::RenderUpdateState() {
  if (state == WindowState_Update) {
    RenderSharedState();
    SubmitUpdate();
  }
}

void DeviceWin::SubmitUpdate() {
  if (Submit()) {
    Device _device = CreateDevice();
    _device.id = previous_device.id;
    Database::Update().Device_(_device);
  }
}

RepairWin::RepairWin() {
  Init();
}

RepairWin::RepairWin(Repair& _repair) {
  Init();
  state = WindowState_Update;
  previous_repair = _repair;
  customer_section.FillBuffersByPhone(_repair.customer);
  device.FillBuffer(_repair.device.name);
  category.FillBuffer(_repair.category.name);
  color.FillBuffer(_repair.color.name);
  sn_imei.FillBuffer(_repair.sn_imei);
  vis_note.FillBuffer(_repair.vis_note);
  hid_note.FillBuffer(_repair.hid_note);
  price = _repair.price;
  items.records = _repair.items.records;
  price_can_be_zero = true;
  repair_state = RoCombo<RepairState>(_("Update repair state"));
  updates_history = Database::Select<RepairUpdate>("note, created_at").From("repair_updates").Where("repair_id = " + std::to_string(_repair.id)).OrderBy("id", 0).All();
}

void RepairWin::Init() {
  open = true;
  customer_section = CustomerWin(TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence);
  price_can_be_zero = true;
  device = DeviceField(_("Model"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence);
  category = SimpleModelField<Category>(_("Category"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_RecordRequired);
  color = RelationalField<Color, DeviceField>(_("Color"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence);
  sn_imei = TextField(_("Serial / IMEI"), 0, TFFlags_EmptyIsError);
  vis_note = TextField(_("Notes for customer"), 0, TFFlags_EmptyIsError);
  hid_note = TextField(_("Notes hidden from customer"), 0, TFFlags_EmptyIsError);
}

void RepairWin::Render() {
  RenderInsertState();
  RenderUpdateState();
}

void RepairWin::UpdatesSection() {
  ImGui::SeparatorText(_("Updates history"));
  float _width = ImGui::GetColumnWidth();
  for (size_t i = 0; i < updates_history.size(); ++i) {
    std::string label = "##Update" + std::to_string(i); // Unique label for each entry
    ImVec2 size = ImGui::CalcTextSize("Wg", NULL, NULL);
    ImVec2 size_curr = ImGui::CalcTextSize(updates_history[i].note.c_str(), NULL, NULL);
    float _height = size_curr.y * 2 < size.y * 5.25 ? size_curr.y * 2: size.y * 5.25;
    std::string _date = Convert::TmToStr(updates_history[i].created_at);
    ImGui::SeparatorTextAligned(_date.c_str(), ImVec2(1.0f, 0.f));
    ImGui::InputTextMultiline(label.c_str(), &updates_history[i].note, ImVec2(_width, _height), ImGuiInputTextFlags_ReadOnly);
  }
}

void RepairWin::ItemAssign() {
  ImGui::SeparatorText(_("Assign parts or items"));
  static InventoryView _view(_("Pick an item"), ViewStateFlags_Select);

  if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
      _view.Render();
  ImGui::EndChild();

  RepairItem& _item = _view.GetSelectedItem();
  if (_item.assign) {
    RepairItem _new = _item;
    //items.push_back(_item);
    Convert::PushBackIfUnique(items.records, _item);
    _item.Clear();
  }
}

void RepairWin::RenderAssignedItems() {
  // Similar table in PurchaseInvoiceWin - can we merge?
  Repair::RenderRepairItemsTable(items, true);
}

void RepairWin::CustomerSection() {
  ImGui::SeparatorColor(_("CUSTOMER"), customer_section.GetError());
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

void RepairWin::PriceSection() {
  ImGui::SeparatorColor(_("PRICE"), price_section_error);
  PriceFeedback(); 
  
  ImGui::BeginWarning(price_section_warning);
  ImGui::InputDouble("##Price", &price, 0.10, 1.0, "%.2f");
  ImGui::EndColor(price_section_warning);
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
  // Set price_section_warning only if there is no price_section_error
  if (!price_section_error) {
    const double epsilon = 0.001; // Precision to two decimal places
    price_section_warning = std::abs(price - items.total.amount) > epsilon;
  }
  else {
    price_section_warning = false; // Reset warning if there is an error
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

void RepairWin::PriceFeedback() {
  if (price_section_error)
    ImGui::Text(_("Price needs to be more than 0"));
  else if (price_section_warning)
    ImGui::Text(_("Price and items total are different"));
  else 
    ImGui::NewLine();
}

void RepairWin::Submit() {
  if (state == WindowState_Insert) {
    ImGui::SeparatorColor(_("SUBMIT"), error);
    ImGui::BeginDisabled(error);
    if (ImGui::Button(_("Submit Repair"))) {
      Insert(CreateRepair());
    }
    ImGui::EndDisabled();
  }
}

Repair RepairWin::CreateRepair() {
  Repair _repair;
  if(state == WindowState_Update)
    _repair.id = previous_repair.id;
  _repair.customer = customer_section.GetEntity();
  // We can change database query below if we needed to get brand and type of the device
  _repair.device = CreateDevice();
  _repair.category = category.GetFromDb();
  _repair.color = color.GetFromDb();
  _repair.sn_imei = sn_imei.Get();
  _repair.vis_note = vis_note.Get();
  _repair.hid_note = hid_note.Get();
  _repair.price = price;
  _repair.repair_state = Database::Get().SimpleModel_<int, RepairState>(2);
  //_repair.cust_device_id = is being set withing repair insertion if device.id is < 0
  _repair.items = items;

  if (state == WindowState_Update)
    _repair.repair_state = repair_state.Get();
  
  return _repair;
}

void RepairWin::RepairValidated() {
  error = customer_section.GetError() || device_section_error || notes_section_error || price_section_error;
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

void RepairWin::RenderInsertState() {
  if (state == WindowState_Insert) {
    ImGui::Begin(_("Repair"), &open);
    if (ImGui::BeginTable("Repair Window", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable)) {
      ImGui::TableNextRow();

      ImGui::TableNextColumn();
      FieldsValidate();
      RepairValidated();
      CustomerSection();
      DeviceSection();
      NotesSection();
      PriceSection();
      Submit();

      ImGui::TableNextColumn();
      ItemAssign();
      RenderAssignedItems();

      ImGui::EndTable();
    }

    ImGui::End();
  }
}

void RepairWin::RenderUpdateState() {
  if (state == WindowState_Update) {
    if (ImGui::BeginTable("Repair Window Edit", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable)) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      FieldsValidate();
      RepairValidated();
      CustomerSection();
      DeviceSection();
      NotesSection();
      PriceSection();
      StateSection();
      Submit();

      ImGui::TableNextColumn();
      ItemAssign();
      RenderAssignedItems();
      
      ImGui::TableNextColumn();
      UpdatesSection();
      ImGui::EndTable();
    }
    StackModal::RenderModal();
  }
}

Repair& RepairWin::GetPrevious() {
  return previous_repair;
}

void RepairWin::Insert(Repair _repair) const {
  _repair.InsertModal();
}

void RepairWin::StateSection() {
  ImGui::SeparatorText(_("State"));
  repair_state.Render();
}

void RepairWin::Update(Repair _repair) const {
  _repair.UpdateModal();
}

void RepairWin::CompareRepairs() {
  Repair _same = CreateRepair();

}

PartsWin::PartsWin() {
  Init();
}

PartsWin::PartsWin(Part _part) {
  Init();
  state = WindowState_Update;
}

void PartsWin::Init() {
  open = true;
  supplier = SimpleModelField<Supplier>(_("Supplier"), 0, TFFlags_HasPopup);
  own_sku_field = OwnSKUField(_("Own SKU"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError);
  name_field = QueriedTextField(_("Item's name"), 0, TFFlags_HasPopup | TFFlags_HasLenValidator | TFFlags_EmptyIsError, "DISTINCT name", "parts", "name");
  color = SimpleModelField<Color>(_("Color"), 0, TFFlags_HasPopup);
  quality = RoCombo<Quality>(_("Choose quality"));
  category = RoCombo<Category>(_("Choose category"));
  location = QueriedTextField(_("Location"), 0, TFFlags_HasPopup, "DISTINCT location", "parts", "location");
  device_filter = QueriedTextField(_("Device's model"), 0, TFFlags_HasPopup, "DISTINCT model", "devices", "model");
  brand_filter = SimpleModelField<Brand>(_("Brand"), 0, TFFlags_HasPopup);
  device_type_filter = SimpleModelField<DeviceType>(_("Device type"), 0, TFFlags_HasPopup);
}

void PartsWin::Render() {
  RenderInsertState();
  RenderUpdateState();
}

void PartsWin::Feedback() {
  FieldsValidate();
  ImGui::NewLine();
  own_sku_field.FeedbackEx();
  name_field.FeedbackEx();
  ImGui::NewLine();
}

void PartsWin::PriceSection(const std::string& _action, Price& _price) {
  std::string _separator;
  std::string _label_1;
  std::string _label_2;
  std::string _label_3;
  if (_action == "purchase") {
    _separator = _("PURCHASE PRICING");
    _label_1 = _("Purchase price (inc. VAT)");
    _label_2 = _("Ex.VAT purchase price");
    _label_3 = _("Price ex.VAT: % .2f");
  }
  if (_action == "sell") {
    _separator = _("SELL PRICING");
    _label_1 = _("Sell price (inc. VAT)");
    _label_2 = _("Ex.VAT sell price");
    _label_3 = _("Price ex.VAT: % .2f");
  }
  ImGui::PushID(_action.c_str());
  ImGui::BeginGroup();
  ImGui::PushItemWidth(100);
  ImGui::SeparatorText(_separator.c_str());
  ImGui::InputDouble(_label_1.c_str(), &_price.amount, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::Checkbox(_label_2.c_str(), &_price.ex_vat);
  ImGui::EndGroup();

  ImGui::BeginGroup();
  ImGui::BeginDisabled(!_price.ex_vat);
  ImGui::PushItemWidth(100);
  ImGui::InputDouble(_("VAT rate"), &_price.vat_rate, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::EndDisabled();
  ImGui::Text((_label_3 + " %.2f").c_str(), _price.ExVat());
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
  ImVec2 _scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 3 + 30);
  ImGui::BeginChild("scrolling", _scrolling_child_size, ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar);
  ImGui::HelpMarker(_("Click to remove device from list of compatible devices."));
  float _window = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
  float _last_btn = ImGui::GetCursorPosX(); // Initialize with the starting cursor position of the _window

  ImGui::SeparatorText(_("Devices"));
  ListEntriesInBox<Device>(_last_btn, _window, cmptble_devices);

  ImGui::SeparatorText(_("Aliases"));
  ListEntriesInBox<Alias>(_last_btn, _window, cmptble_aliases);

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
  if (state == WindowState_Insert) {
    if (ImGui::ColorButtonEx(_("Submit"), 0.2f, error)) {
      Insert(CreatePart());
    }
    // Since we are running this window as modal, we use StackModal
    StackModal::RenderModal();
  }
}

Part PartsWin::CreatePart() {
  Part _part;
  _part.name = name_field.Get();
  _part.own_sku = own_sku_field.Get();
  _part.quality = quality.Get();
  _part.category = category.Get();
  _part.sell_price = sell_price.amount;
  _part.sell_price_ex_vat = sell_price.ExVat();
  _part.color = color.GetFromDb();
  _part.quantity = quantity;
  _part.purch_price = purch_price.amount;
  _part.purch_price_ex_vat = purch_price.ExVat();
  _part.vat = purch_price.vat_rate;
  _part.location = location.Get();
  _part.reserved_quantity = 0;
  _part.cmptble_devices = std::move(cmptble_devices);
  _part.cmptble_aliases = std::move(cmptble_aliases);
  return _part;
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

void PartsWin::LoadDevices() { // edit so we won't be looping through devices to get aliases and reduce database calls;
    devices.clear();

    std::string _brand_id_str = "";
    int _brand_id = brand_filter.GetFromDb().id;
    if (_brand_id > 0)
      _brand_id_str = "brand_id = " + std::to_string(_brand_id);

    std::string _type_id_str = "";
    int _type = device_type_filter.GetFromDb().id;
    if (_type > 0)
      _type_id_str = "type_id = " + std::to_string(_type);

    devices = Database::Select<Device>().From().Where("model").Like(device_filter.Get())
      .And(_brand_id_str)
      .And(_type_id_str)
      .All();

  for (auto& device : devices) {
    std::string _id_str = std::to_string(device.id);
    device.aliases = (Database::Select<Alias>().From().Where("model_id = " + _id_str).All());
  }

}

void PartsWin::FieldsValidate() {
  error = own_sku_field.error || name_field.error;
}

void PartsWin::RenderSharedBetweenStates() {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();

  supplier.Render();
  own_sku_field.Render();
  name_field.Render();
  PriceSection("purchase", purch_price);
  PriceSection("sell", sell_price);
  QuantitySection();

  ImGui::TableNextColumn();

  color.Render();
  quality.Render();
  category.Render();
  location.Render();
  CompatibleEntriesBox();
  Submit();

  Feedback();

}

void PartsWin::RenderInsertState() {
  if (state != WindowState_Insert)
    return;

  if (!open)
    return;

  ImGui::OpenPopup(_("Insert item"));

  if (ImGui::BeginPopupModal(_("Insert item"), &open)) {
    if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_SizingStretchProp)) {
      RenderSharedBetweenStates();
      ImGui::EndTable();
    }
    Filters();
    CompatibleTablePicker();
    ImGui::EndPopup();
  }
}

Part PartsWin::GetPrevious() {
  return previous_part;
}

void PartsWin::RenderUpdateState() {
  if (state != WindowState_Update)
    return;

  if (ImGui::BeginTable("Update item", 2, ImGuiTableFlags_SizingStretchProp)) {
    RenderSharedBetweenStates();
    ImGui::EndTable();
  }

  
}

void PartsWin::Insert(Part _part) const {
  _part.InsertModal();
}

CustomDeviceWin::CustomDeviceWin() { 
  Init();
}

void CustomDeviceWin::Init() {
  devices = Database::Select<CustomDevice>("DISTINCT model")
    .From()
    .All();
  open = true;
}

void CustomDeviceWin::Render(){
  if (!open)
    return;

  ImGui::OpenPopup(_("Change custom device to device"));
  if (ImGui::BeginPopupModal(_("Change custom device to device"), &open)) {

    RoTable::SimpleModel(devices);
    StackModal::RenderModal();
    ImGui::EndPopup();
  }
}

template<typename T>
void SimpleModelWin<T>::Render() {
  ImGui::OpenPopup(model.window_title.c_str());
  if (ImGui::BeginPopupModal(model.window_title.c_str(), &open)) {
    RoTable::SimpleModel<T>(values);
    name.Render();
    ImGui::Text("%s", _("Please right-click to edit or delete value"));
    if (ImGui::Button(_("Add"))) {
      model.name = name.Get();
      Database::Insert().OfSimpleModel(model);
      LoadData();
    }
    StackModal::RenderModal();
    ImGui::EndPopup();
  }
}

template class SimpleModelWin<Brand>;
template class SimpleModelWin<RepairState>;
template class SimpleModelWin<Category>;
template class SimpleModelWin<PaymentMethod>;
template class SimpleModelWin<DeviceType>;
template class SimpleModelWin<Color>;
template class SimpleModelWin<Quality>;
 
PurchaseInvoiceWin::PurchaseInvoiceWin()
  : invoice_number(_("Invoice number"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError, "invoice_number"),
   external_id(_("External ID"), 0, TFFlags_HasPopup | TFFlags_DuplicateWarning , "external_id", "purchase_invoices", "external_id"),
   supplier_field(_("Supplier"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError | TFFlags_AllowDbPresence)
{
  Init();
} 

void PurchaseInvoiceWin::Init() {
  open = true;
  ImGui::SetDateToday(&create_date.date);
  ImGui::SetDateToday(&purchase_date.date);
  ImGui::SetDateToday(&arrival_date.date);
  std::string _last_number = Database::Select<std::string>("invoice_number").From("purchase_invoices").Last().One();
  invoice_number.FillBuffer(_last_number);
}

void PurchaseInvoiceWin::Render() {
  ImGui::OpenPopup(_("Purchase invoice"));
  if (ImGui::BeginPopupModal(_("Purchase invoice"), &open)) {
    Feedback();
    RenderInvoiceHeader();
    RenderInvoiceItems();
    RenderAddItemButton();
    Submit();
    ImGui::EndPopup();
  }
}

void PurchaseInvoiceWin::RenderInvoiceHeader() {
  ImGui::SeparatorText(_("Invoice details"));
  if (ImGui::BeginTable("Invoice Details", 3)) {
    RenderInvoiceNumber();
    RenderDateFields();
    ImGui::EndTable();
  }

  ImGui::SeparatorText(_("Supplier details"));
  if (ImGui::BeginTable("Supplier Details", 3)) {
    RenderSupplierField();
    ImGui::EndTable();
  }
}

void PurchaseInvoiceWin::RenderInvoiceNumber() {
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  static bool _copy = false;
  if (external_id.Render() && _copy)
    invoice_number.FillBuffer(external_id.Get());

  ImGui::TableNextColumn();
  ImGui::PushItemWidth(100);
  if (ImGui::Checkbox(_("Copy from external"), &_copy)) {
    invoice_number.Clear();
    if (_copy)
      invoice_number.FillBuffer(external_id.Get());
  }
  if (ImGui::BeginCombo("##prefix", "PI/")) {
    ImGui::EndCombo();
  }
  ImGui::PopItemWidth();
  ImGui::SameLine();
  ImGui::BeginDisabled(_copy);
  invoice_number.Render();
  ImGui::EndDisabled();
}

void PurchaseInvoiceWin::RenderDateFields() {
  ImGui::TableNextColumn();
  
  ImGui::BeginWarning(create_date.warning);
  ImGui::DateChooser(_("Insert date"), create_date.date);
  ImGui::EndColor(create_date.warning);

  ImGui::BeginWarning(purchase_date.warning);
  ImGui::DateChooser(_("Purchase date"), purchase_date.date);
  ImGui::EndColor(purchase_date.warning);

  ImGui::BeginWarning(arrival_date.warning);
  ImGui::DateChooser(_("Arrival date"), arrival_date.date);
  ImGui::EndColor(arrival_date.warning);
}

void PurchaseInvoiceWin::RenderSupplierField() {
  ImGui::TableNextColumn();
  ImGui::Text(_("Supplier"));

  ImGui::TableNextColumn();
  if (supplier_field.Render()) {
    supplier = supplier_field.GetFromDb();
  }

  ImGui::TableNextColumn();
  AddSupplierBtn();

  ImGui::TableNextColumn();
  ImGui::Text(_("Supplier's address"));

  ImGui::TableNextColumn();
  ImGui::TextWrapped("%s", supplier.address.ToString(", ", "right").c_str());

  ImGui::TableNextColumn();

}

void PurchaseInvoiceWin::AddSupplierBtn() {
  ImGui::Button(_("Add Supplier"));
  if (ImGui::BeginPopupContextItem("##Supplier", ImGuiPopupFlags_MouseButtonLeft)) {
    static SupplierWin _supplier_win;
    _supplier_win.Feedback();
    _supplier_win.InputFields();
    if (ImGui::Button(_("Add"))) {
      supplier = _supplier_win.GetEntity();
      supplier_field.FillBuffer(supplier.name);
      supplier.InsertToDb();
      supplier_field.Validate();
      _supplier_win.Clear();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void PurchaseInvoiceWin::RenderInvoiceItems() {
  ImGui::SeparatorText(_("Invoice items"));

  static ImGuiTableFlags _flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;
  if (ImGui::BeginTable("InvoiceItems", 9, _flags)) {
    RenderInvoiceTableHeaders();
    RenderInvoiceTableRows();
    ImGui::EndTable();
  }
}

void PurchaseInvoiceWin::RenderInvoiceTableHeaders() {
  std::vector<std::string> headers = { _("ID"), _("Name"), _("Supplier SKU"), _("Own SKU"), _("Quantity"), _("Purchase price. ex VAT"), _("VAT"), _("Total Net"), _("Total") };
  for (const auto& header : headers) {
    ImGui::TableSetupColumn(header.c_str());
  }
  ImGui::TableHeadersRow();
}

void PurchaseInvoiceWin::RenderInvoiceTableRows() {
  int i = 0;
  for (auto it = items.begin(); it != items.end(); ) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    std::string _label = std::to_string(++i);
    ImGui::Selectable(_label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns);
    if (ImGui::BeginPopupContextItem(_label.c_str() , ImGuiPopupFlags_MouseButtonRight)) {
      if (ImGui::Button(_("Remove"))) {
        it = items.erase(it); // Erase the element and update the iterator
        ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
        continue; // Skip the increment of the iterator
      }
      ImGui::EndPopup();
    }

    ImGui::TableNextColumn();
    ImGui::TextWrapped("%s", it->name.c_str());
    ImGui::TableNextColumn();
    ImGui::TextWrapped("%s", it->supplier_sku.c_str());
    ImGui::TableNextColumn();
    ImGui::TextWrapped("%s", it->own_sku.c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%d", it->quantity);
    ImGui::TableNextColumn();
    ImGui::Text("%.2f", it->price.amount);
    ImGui::TableNextColumn();
    ImGui::Text("%.2f", it->price.vat_rate);
    ImGui::TableNextColumn();
    ImGui::Text("%.2f", it->total_net);
    ImGui::TableNextColumn();
    ImGui::Text("%.2f", it->total);

    ++it; // Increment the iterator only if no element was erased
  }
}

void PurchaseInvoiceWin::RenderAddItemButton() {
  ImGui::Button(_("Add item"));
  if (ImGui::BeginPopupContextItem("##empty", ImGuiPopupFlags_MouseButtonLeft)) {
    static InvoiceItemPicker _item_picker;
    _item_picker.Render();
    ImGui::BeginDisabled(_item_picker.error);
    if (ImGui::Button(_("Add to invoice"))) {
      items.push_back(_item_picker.GetPart());
      _item_picker.Clear();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();
    ImGui::EndPopup(); 
  }
}

void PurchaseInvoiceWin::Submit() {
  ImGui::BeginDisabled(error);
  if (ImGui::Button(_("Create Invoice"))) {
    PurchaseInvoice _invoice;
    _invoice.name = invoice_number.Get();
    _invoice.external_id = external_id.Get();
    _invoice.purchased_at = purchase_date.date;
    _invoice.arrived_at = arrival_date.date;
    _invoice.created_at = create_date.date;
    _invoice.supplier = supplier;
    _invoice.items = items;
    _invoice.InsertToDb();

    ResetFields();
  }
  ImGui::EndDisabled();
}

void PurchaseInvoiceWin::ResetFields() {
  invoice_number.Clear();
  external_id.Clear();
  create_date.Clear();
  purchase_date.Clear();
  arrival_date.Clear();
  supplier_field.Clear();
  supplier.Clear();
  items.clear();
  error = false;
}

void PurchaseInvoiceWin::Feedback() {
  FieldsValidate();
  invoice_number.FeedbackEx();
  external_id.FeedbackEx();
  supplier_field.FeedbackEx();
  if (create_date.warning || purchase_date.warning || arrival_date.warning ) {
    ImGui::Text("%s", _("Some dates are in the future")); ImGui::SameLine();
  }
  ImGui::NewLine();
}

void PurchaseInvoiceWin::FieldsValidate() {
  create_date.warning = ImGui::DateInvalid(create_date.date);
  purchase_date.warning = ImGui::DateInvalid(purchase_date.date);
  arrival_date.warning = ImGui::DateInvalid(arrival_date.date);

  error = invoice_number.error || supplier_field.error || items.empty();
}

SupplierWin::SupplierWin() 
  : name(_("Name"), 0, TFFlags_HasPopup | TFFlags_EmptyIsError)
{
  Init();
}

void SupplierWin::Init() {
  open = true;
  address.resize(5);
  // Initialize address fields
  for (int i = 0; i < 5; i++) {
    std::string label = _("Address Line ") + std::to_string(i + 1);
    address[i] = TextField(label);
  }
}

void SupplierWin::Render() {
  ImGui::OpenPopup(_("Supplier"));
  if (ImGui::BeginPopupModal(_("Supplier"), &open)) {
    Feedback();
    InputFields();
    Submit();
    ImGui::EndPopup();
  }
}

void SupplierWin::Feedback() {
  FieldsValidate();
  name.Feedback();
  ImGui::NewLine();
}

void SupplierWin::InputFields() {
  name.Render();
  for (auto& line : address) {
    line.Render();
  }
}

Supplier SupplierWin::GetEntity() {
  Supplier _supplier;
  _supplier = Database::Select<Supplier>().From().Where("supplier", name.Get()).One();
  if (_supplier.id > 0) {
    return _supplier;
  }
  _supplier.id = -1;
  _supplier.name = name.Get();
  _supplier.address.SetLines(address);
  return _supplier;
}

void SupplierWin::Clear() {
  name.Clear();
  for (auto& line : address) {
    line.Clear();
  }
  error = true;
}

void SupplierWin::Submit() {
  ImGui::BeginDisabled(error);
  if (ImGui::Button(_("Submit"))) {
    Supplier _supplier = GetEntity();
    _supplier.InsertToDb();
  }
  ImGui::EndDisabled();
}

void SupplierWin::FieldsValidate() {
  error = name.error;
}

