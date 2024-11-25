#include "repair.h"
//#include "repair.h"
//
//Repair::Repair() { }

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
