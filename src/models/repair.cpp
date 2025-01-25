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

void Repair::RepairItemsTable(ItemsContainer<RepairItem>& _items, const bool _removable) {
  // Similar table in PurchaseInvoiceWin - can we merge?
  ImGui::SeparatorText(_("Assigned items"));
  double _total_net = 0;
  _items.total.amount = 0;
  static ImGuiTableFlags _flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;
  if (ImGui::BeginTable("Assigned items", 7, _flags)) {
    std::vector<std::string> headers = { _("ID"), _("Name"), _("Quantity"), _("Sell price ex. VAT"), _("VAT"), _("Total Net"), _("Total") };
    for (const auto& header : headers) {
      ImGui::TableSetupColumn(header.c_str());
    }
    ImGui::TableHeadersRow();
    int i = 0;
    for (auto it = _items.records.begin(); it != _items.records.end(); ) {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      std::string _label = std::to_string(++i);
      ImGui::Selectable(_label.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups);
      if (_removable && ImGui::BeginPopupContextItem(_label.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
        //if (ImGui::BeginPopupContextItem(_label.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
          if (ImGui::Button(_("Remove"))) {
            it = _items.records.erase(it); // Erase the element and update the iterator
            ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
            continue; // Skip the increment of the iterator
          }
          ImGui::EndPopup();
        
      }
      ImGui::TableNextColumn();
      ImGui::TextWrapped("%s", it->part.name.c_str());
      ImGui::TableNextColumn();
      ImGui::TextWrapped("%d", it->quantity);
      ImGui::TableNextColumn();
      ImGui::TextWrapped("%.2f", it->part.sell_price_ex_vat);
      ImGui::TableNextColumn();
      ImGui::Text("%2.0f", it->part.vat);
      ImGui::TableNextColumn();
      ImGui::Text("%.2f", it->total_net);
      _total_net += it->total_net;
      ImGui::TableNextColumn();
      ImGui::Text("%.2f", it->total);
      _items.total.amount += it->total;
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
  RepairItemsTable(items);
}

void Repair::InsertModal() {
  ModalConfig _config;
  _config.Title(_("Insert new repair?"));
  RepairModal _modal(*this, _config);
  ModalManager::SetModal(_modal);
}

void Repair::InsertToDb()  {
  Database::Insert().Repair_(*this);
}
