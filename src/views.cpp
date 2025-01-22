#include "views.h"
#include "RoLocalization.h"
#include "conversion.h"
#include "database.h"

CustomerView::CustomerView() 
  : BaseTableView<Customer>(
    "Customers view",
    5,
    {
      {"id", "ID"},
      {"phone", "Phone"},
      {"name", "Name"},
      {"surname", "Surname"},
      {"email", "Email"}
    }
  ) 
  {
  std::cout << "Customers view created" << std::endl;
  LoadData();
  }

CustomerView::~CustomerView() {
  std::cout << "Customers view destroyed" << std::endl;
}


void CustomerView::DefaultRenderItem(const Customer& customer) {
  ImGui::TableNextColumn();
  ImGui::Text("%d", customer.id);

  ImGui::TableNextColumn();

  if (customer.HasRepairs()) {
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.7f, 0.3f, 0.3f, 0.65f));
  }

  ImGui::Selectable(customer.phone.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  if (customer.HasRepairs()) {
    ImGui::PopStyleColor();
    ImGui::SetItemTooltip("Can't remove customer as long as there is a related repair.", ImGui::GetStyle().HoverDelayShort);
  }
  else {
    if (ImGui::BeginPopupContextItem()) {


      if (ImGui::Button(_("Remove customer?")))
        customer.RemoveModal();

      ImGui::EndPopup();
    }
  }

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.name.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.surname.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", customer.email.c_str());

}

void CustomerView::LoadData(const std::string& _orderby, const int& _direction) {
  data = Database::Select<Customer>("c.*, COUNT(repairs.id) AS has_repairs")
    .From("customers c")
    .LeftJoin("repairs")
    .On("c.id = repairs.customer_id")
    .GroupBy("c.id")
    .OrderBy(_orderby, _direction)
    .All();
}

RepairView::RepairView()
  : BaseTableView<Repair>(
    "Repairs view",
    13,
    {
        {"id", "ID"},
        {"phone", "Cust. Phone"},
        {"name", "Cust. Name"},
        {"device", "Device"},
        {"category", "Category"},
        {"visible_desc", "Notes"},
        {"hidden_desc", "Hidden note"},
        {"price", "Price"},
        {"state", "State"},
        {"sn_imei", "SN / IMEI"},
        {"created_at", "Created at"},
        {"updated_at", "Updated at"},
        {"finished_at", "Finished at"}
    }
  ),
  phone(
    _("Search by phone"),
    ImGuiInputTextFlags_CharsDecimal,
    TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence
  ),
  id_filter(
    _("Search by ID"),
    ImGuiInputTextFlags_CharsHexadecimal,
    TFFlags_HasPopup,
    "id",
    "repairs",
    "id"
  ),
  timelines{
      {"created_at", "Created at"},
      {"updated_at", "Updated at"},
      {"finished_at", "Finished at"}
  },
  timeline_combo(timelines),
  states(Database::Select<RepairState>().From().All())
  { }

void RepairView::DefaultRenderItem(const Repair& _repair) {
  ImGui::TableNextColumn();
  std::string _id_str = std::to_string(_repair.id);
  ImGui::Selectable(_id_str.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.phone.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.customer.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.device.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.category.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.vis_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.hid_note.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _repair.price);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.repair_state.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _repair.sn_imei.c_str());

  ImGui::TableNextColumn();
  std::string _created_at = Convert::TmToStr(_repair.created_at);
  ImGui::Text("%s", _created_at.c_str());

  ImGui::TableNextColumn();
  std::string _updated_at = Convert::TmToStr(_repair.updated_at);
  ImGui::Text("%s", _updated_at.c_str());

  ImGui::TableNextColumn();
  std::string _finished_at = Convert::TmToStr(_repair.finished_at);
  ImGui::Text("%s", _finished_at.c_str());

}

void RepairView::LoadData(const std::string& _orderby, const int& _direction) {
  data = Database::Select<Repair>("r.*, c.phone, c.name, rc.category, rs.state, ")
    .Coalesce(" (d.model, cd.model) AS model")
    .From("repairs r")
    .LeftJoin("customers c").On("c.id = r.customer_id")
    .LeftJoin("devices d").On("d.id = r.model_id")
    .LeftJoin("custom_devices cd").On("cd.id = r.cust_device_id")
    .LeftJoin("repair_categories rc").On("rc.id = r.category_id")
    .LeftJoin("repair_states rs").On("rs.id = r.repair_state_id")
    .Where("c.phone")
    .Like(phone.Get())
    .And("r.id")
    .Like(id_filter.Get())
    .And(state_query)
    .And(timeline_combo.Get().column)
    .Date(date.GetForSQL())
    .OrderBy(_orderby, _direction)
    .All();
}

void RepairView::Filters() {
  ImGui::SeparatorText(_("Filter by details"));
  if (phone.Render())
    LoadData();
  if (id_filter.Render()) {
    LoadData();
  }
  ImGui::SeparatorText(_("Filter by dates"));
  if (date.Render()) {
    LoadData();
  }
  if (timeline_combo.Render()) {
    LoadData();
  }
  ImGui::SeparatorText(_("Results"));
  if (states.Render()) {
    int _id = states.Get().id;
    if (_id <= 1) {
      state_query = "";
    }
    else {
      state_query = "r.repair_state_id =" + std::to_string(_id);
    }
    LoadData();
  }
}

InventoryView::InventoryView()
  : BaseTableView<Part>("Inventory view", 16, {})
{
  Init("Inventory view", ViewStateFlags_Default);
}

InventoryView::InventoryView(const std::string& _window_id, ViewStateFlags _flags)
  : BaseTableView<Part>(_window_id, 16, {})
{
  Init(_window_id, _flags, false);
}

RepairItem& InventoryView::GetSelectedItem() {
  if (config.flags & ViewStateFlags_Select)
    return repair_item;
}

void InventoryView::Init(const std::string& _window_id, ViewStateFlags _flags, const bool& _is_window) {
  config.window_id = _window_id;
  config.max_columns = 16;
  config.headers = {
    { "id", "ID"},
    { "name", "Name"},
    { "own_sku", "Own SKU"},
    { "quality", "Quality"},
    { "category", "Category"},
    { "sell_price", "Sell Price"},
    { "sell_price_ex_vat", "Sell Price ex.VAT"},
    { "color", "Color"},
    { "quantity", "Quantity"},
    { "purch_price", "Purchase price"},
    { "purch_price_ex_vat", "Purchase price ex.VAT"},
    { "vat", "VAT"},
    { "location", "Location"},
    { "reserved_quantity", "Reserved Quantity"},
    { "created_at", "Created at" },
    { "updated_at", "Updated at" }
  };
  config.is_window = _is_window;
  config.flags = _flags;
}

void InventoryView::DefaultRenderItem(const Part & _part) {
  ImGui::TableNextColumn();
  ActionsOnTable(const_cast<Part&>(_part));

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.own_sku.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.quality.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.category.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.sell_price);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.sell_price_ex_vat);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.color.name.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%d", _part.quantity);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.purch_price);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.purch_price_ex_vat);

  ImGui::TableNextColumn();
  ImGui::Text("%.2f", _part.vat);

  ImGui::TableNextColumn();
  ImGui::Text("%s", _part.location.c_str());

  ImGui::TableNextColumn();
  ImGui::Text("%d", _part.reserved_quantity);

  ImGui::TableNextColumn();
  std::string _created_at = Convert::TmToStr(_part.created_at);
  ImGui::Text("%s", _created_at.c_str());

  ImGui::TableNextColumn();
  std::string _updated_at = Convert::TmToStr(_part.updated_at);
  ImGui::Text("%s", _updated_at.c_str());
}

void InventoryView::DefaultAction(Part& _item) {
  std::string _id_str = std::to_string(_item.id);
  ImGui::Selectable(_id_str.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);
}

void InventoryView::SelectAction(Part& _item) {
  std::string _id_str = std::to_string(_item.id);
  if (ImGui::Selectable(_id_str.c_str(), repair_item.id == _item.id, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap
    | ImGuiSelectableFlags_AllowDoubleClick  )) {
      if (ImGui::IsMouseDoubleClicked(0)) {
        repair_item.id = _item.id;
        repair_item.part = _item;
      }
  }
  if (repair_item.id == _item.id) {
    if (ImGui::BeginPopupContextItem(_id_str.c_str(), ImGuiPopupFlags_MouseButtonLeft)) {
      static RepairItemPicker _picker(repair_item);
      _picker.Render();
      ImGui::EndPopup();
    }
  }
}

void InventoryView::LoadData(const std::string& _orderby, const int& _direction) {
  std::string own_sku = item_filter.GetOwnSKU();
  data = Database::Select<Part>("DISTINCT p.*, q.quality , rc.category , c.color  ")
    .From("parts p")
    .LeftJoin("qualities q").On("q.id = p.quality_id")
    .LeftJoin("repair_categories rc").On("rc.id = p.category_id")
    .LeftJoin("colors c").On("c.id = p.color_id")
    .LeftJoin("part_model pm").On("pm.part_id = p.id")
    .LeftJoin("part_model_alias pma").On("pma.part_id = p.id")
    .Where("p.own_sku")
    .Like(own_sku)
    .And("p.name").Like(item_filter.GetName())
    .And("q.quality").Like(item_filter.GetQuality())
    .And("rc.category").Like(item_filter.GetCategory())
    .And(item_filter.GetDevice())
    .And(item_filter.GetAlias())
    .OrderBy(_orderby, _direction)
    .All();

}

void InventoryView::Filters() {
  if(item_filter.Render())
    LoadData();
}

Part InventoryView::SelectState() {
    return Part();
}

DevicesView::DevicesView()
  : BaseTableView<Device>(
    "Devices view",
    3,
    {
        { "name", "Name"},
        { "brand", "Brand"},
        { "type", "Type"},
    }
    ),
  device_filter(
    _("Model"),
    0,
    TFFlags_HasPopup | TFFlags_NeverAnError | TFFlags_AllowDbPresence, "model ", "devices", "model "),
  device_type_filter(_("Device type"), RoComboFlags_HasNone),
  brand_filter(_("Brand"), RoComboFlags_HasNone)
{
  LoadData();
}

void DevicesView::DefaultRenderItem(const Device& _device) {

  ImGui::TableNextColumn();
  bool _open = ImGui::TreeNodeEx(_device.name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
  if (_open) {
    // Render Colors
    std::string _colors_node = std::string(_("Colors")) + "(" + std::to_string(_device.colors.size()) + ")";
    if (ImGui::TreeNode(_colors_node.c_str())) {
      for (const auto& color : _device.colors) {
        ImGui::BulletText("%s", color.name.c_str());
      }
      ImGui::TreePop();
    }

    // Render Aliases
    std::string _aliases_node = std::string(_("Aliases")) + "(" + std::to_string(_device.aliases.size()) + ")";
    if (ImGui::TreeNode(_aliases_node.c_str())) {
      for (const auto& alias : _device.aliases) {
        ImGui::BulletText("%s", alias.name.c_str());
      }
      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
  ImGui::TableNextColumn();
  ImGui::Text("%s", _device.brand.name.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", _device.type.name.c_str());
}

void DevicesView::LoadData(const std::string& _orderby, const int& _direction) {
  std::string _device_type = device_type_filter.Get() ? "dt.id = " + std::to_string(device_type_filter.Get().id) : "";
  std::string _brand = brand_filter.Get() ? "b.id = " + std::to_string(brand_filter.Get().id) : "";
  data = Database::Select<Device>("d.*, b.brand, dt.type")
    .From("devices d")
    .LeftJoin("brands b").On("b.id = d.brand_id")
    .LeftJoin("device_types dt").On("dt.id = d.type_id")
    .Where("model")
    .Like(device_filter.Get())
    .And(_device_type)
    .And(_brand)
    .OrderBy(_orderby, _direction)
    .All();

  for (auto& _device : data) {
    std::string _id_str = std::to_string(_device.id);
    _device.aliases = (Database::Select<Alias>().From().Where("model_id = " + _id_str).All());
    _device.colors = (Database::Select<Color>("c.*")
      .From("colors c")
      .InnerJoin("model_colors mc").On("c.id = mc.color_id")
      .Where("mc.model_id = " + _id_str)

      .All());
  }
}

void DevicesView::Filters() {
  if(device_filter.Render())
    LoadData();
  if (device_type_filter.Render()) {
    LoadData();
  }
  if (brand_filter.Render()) {
    LoadData();
  }
}

