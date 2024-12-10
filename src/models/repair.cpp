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
