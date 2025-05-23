#include "repair.h"
#include "../../src/RoLocalization.h"
#include "../../src/database.h"
#include "../../src/modal.h"

const std::string Repair::ToString() const {
  return std::to_string(id) 
    + " | \n Customer: " + customer.ToString()
    + " | \n Device: " + device.ToString() 
    + " | \n Category: " + category.ToString() 
    + " | \n Color: "+ color.ToString() 
    + " | \n SN IMEI: " + sn_imei 
    + " | \n Vis Note: " + vis_note 
    + " | \n Hid note: " + hid_note 
    + " | \n Price: " + std::to_string(price) 
    + " | \n Repair State: " + repair_state.ToString()
    + "\n";
}

void Repair::RenderRepairItemsTable(ItemsContainer<RepairItem>& _items, const bool _removable, const ItemsContainer<RepairItem>& _second) {
  // Similar table in PurchaseInvoiceWin - can we merge?
  ImGui::SeparatorText(_("Assigned items"));
  double _total_net = 0;
  //_items.total.amount = 0;
  static ImGuiTableFlags _flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;
  if (ImGui::BeginTable("Assigned items", 7, _flags)) {
    std::vector<std::string> headers = { _("ID"), _("Name"), _("Quantity"), _("Sell price ex. VAT"), _("VAT"), _("Total Net"), _("Total") };
    for (const auto& header : headers) {
      ImGui::TableSetupColumn(header.c_str());
    }
    ImGui::TableHeadersRow();


    int _index = 0;
    for (auto it = _items.records.begin(); it != _items.records.end(); ) {
      bool _compare = (_index < _second.records.size());

      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      
      std::string _label = std::to_string(++_index);
      ImGui::Selectable(_label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups);
      if (_removable && ImGui::BeginPopupContextItem(_label.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        //if (ImGui::BeginPopupContextItem(_label.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
          if (ImGui::Button(_("Remove"))) {
            _items.Erase(it);
            //it = _items.records.erase(it); // Erase the element and update the iterator
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            continue; // Skip the increment of the iterator
          }
          ImGui::EndPopup();
        

      }
      // Use RenderItemsTableRow for each column but index
      RenderItemsTableRow(it->part.name, _compare ? _second.records[_index - 1].part.name : "", "%s", _compare);
      RenderItemsTableRow(it->quantity, _compare ? _second.records[_index - 1].quantity : 0, "%d", _compare);
      RenderItemsTableRow(it->part.sell_price_ex_vat, _compare ? _second.records[_index - 1].part.sell_price_ex_vat : 0.0, "%.2f", _compare);
      RenderItemsTableRow(it->part.vat, _compare ? _second.records[_index - 1].part.vat : 0.0, "%2.0f", _compare);
      
      // Old
      // RenderItemsTableRow(it->total_net, _compare ? _second.records[_index - 1].total_net : 0.0, "%.2f", _compare);
      // RenderItemsTableRow(it->total, _compare ? _second.records[_index - 1].total : 0.0, "%.2f", _compare);

      // New
      RenderItemsTableRow(it->price.ExVat(), _compare ? _second.records[_index - 1].price.ExVat() : 0.0, "%.2f", _compare);
      RenderItemsTableRow(it->price.amount, _compare ? _second.records[_index - 1].price.amount : 0.0, "%.2f", _compare);
      _total_net += it->price.ExVat();

      //_items.total.amount += it->total;
      //_total_net += it->total_net;

      ++it;
    }
    ImGui::EndTable();
  }
  ImGui::Text(_("Total Net: %.2f"), _total_net);
  ImGui::Text(_("Total: %.2f"), _items.total.amount);
}


void Repair::View() {
  ImGui::SeparatorText(_("CUSTOMER"));
  customer.View();
  ImGui::SeparatorText(_("REPAIR DETAILS"));
  ImGui::TextWrapped("Model:              %s", device.name.c_str());
  ImGui::TextWrapped("Category:           %s", category.name.c_str());
  ImGui::TextWrapped("Color:              %s", color.name.c_str());
  ImGui::TextWrapped("SN_IMEI:            %s", sn_imei.c_str());
  ImGui::TextWrapped("Note for customer:  %s", vis_note.c_str());
  ImGui::TextWrapped("Note for store:     %s", hid_note.c_str());
  ImGui::TextWrapped("State:              %s", repair_state.name.c_str());
  ImGui::TextWrapped("Price:              %.2f", price);
  RenderRepairItemsTable(items, false);
}

// Used to compare two repairs when we are updateding an existing repair
void Repair::View(const Repair& _previous) {
  ImGui::SeparatorText(_("CUSTOMER"));
  customer.View(_previous.customer);
  ImGui::SeparatorText(_("REPAIR DETAILS"));
  
  ImGui::TextWrappedColor(device.name != _previous.device.name,             "Model:              %s", device.name.c_str());
  ImGui::TextWrappedColor(category.name != _previous.category.name,         "Category:           %s", category.name.c_str());
  ImGui::TextWrappedColor(color.name != _previous.color.name,               "Color:              %s", color.name.c_str());
  ImGui::TextWrappedColor(sn_imei != _previous.sn_imei,                     "SN_IMEI:            %s", sn_imei.c_str());
  ImGui::TextWrappedColor(vis_note != _previous.vis_note,                   "Note for customer:  %s", vis_note.c_str());
  ImGui::TextWrappedColor(hid_note != _previous.hid_note,                   "Note for store:     %s", hid_note.c_str());
  ImGui::TextWrappedColor(price != _previous.price,                         "Price:              %.2f", price);
  ImGui::TextWrappedColor(repair_state.name != _previous.repair_state.name, "Repair State:       %s", repair_state.name.c_str());
  RenderRepairItemsTable(items, false, _previous.items);
}

void Repair::InsertModal() {
  ModalConfig _config;
  _config.Title(_("Insert new repair?"))
    .State(ModalState_Insert);
  RepairModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

void Repair::InsertToDb()  {
  Database::Insert().Repair_(*this);
}
 
void Repair::UpdateModal() {
  ModalConfig _config;
  _config.Title(_("Edit repair?"))
    .State(ModalState_UpdateWindow);
  RepairModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

void Repair::UpdateToDb() {
  Database::Update().Repair_(*this);
}

std::string Repair::UpdateString(const Repair& _other) {
  std::stringstream changes;
  if (customer != _other.customer) {
    if (_other.customer.phone != customer.phone) {
      changes << "Phone: " << _other.customer.phone << " -> " << customer.phone << "\n";
    }
    if (_other.customer.name != customer.name) {
      changes << "Name: " << _other.customer.name << " -> " << customer.name << "\n";
    }
    if (_other.customer.surname != customer.surname) {
      changes << "Surname: " << _other.customer.surname << " -> " << customer.surname << "\n";
    }
    if (_other.customer.email != customer.email) { 
      changes << "Email: " << _other.customer.email << " -> " << customer.email << "\n";
    }
  }
  if (_other.device.name != device.name) {
    changes << "Model: " << _other.device.name << " -> " << device.name << "\n";
  }
  if (_other.category != category) {
    changes << "Category: " << _other.category.name << " -> " << category.name << "\n";
  }
  if (_other.color != color) {
    changes << "Color : " << _other.color.name << " -> " << color.name << "\n";
  }
  if (_other.sn_imei != sn_imei) {
    changes << "SN/IMEI: " << _other.sn_imei << " -> " << sn_imei << "\n";
  }
  if (_other.vis_note != vis_note) {
    changes << "Visible note: " << _other.vis_note << " -> " << vis_note << "\n";
  }
  if (_other.hid_note != hid_note) {
    changes << "Hidden note: " << _other.hid_note << " -> " << hid_note << "\n";
  }
  if (_other.price != price) {
    changes << "Price: " << _other.price << " -> " << price << "\n";
  }
  if (_other.repair_state != repair_state) {
    changes << "Repair state: " << _other.repair_state.name << " -> " << repair_state.name << "\n";
  }
  last_update = changes.str();
  std::cout << changes.str() << std::endl;
  //if (items != _other.items) {
  //  changes << "Items: \n";
  //  for (const auto& item : items.records) {
  //    if (std::find(_other.items.records.begin(), _other.items.records.end(), item) == _other.items.records.end()) {
  //      changes << "Added: " << item.part.name << "\n";
  //    }
  //  }
  //  for (const auto& item : _other.items.records) {
  //    if (std::find(items.records.begin(), items.records.end(), item) == items.records.end()) {
  //      changes << "Removed: " << item.part.name << "\n";
  //    }
  //  }
  //}
  return std::string();
}
