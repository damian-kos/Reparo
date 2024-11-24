#pragma once
#include "customer.h"

class Repair  {
  static constexpr std::string_view table = "repairs";

private:
  int id;
  int customer_id;
  int device_id;
  int category_id;
  int color_id;
  std::string vis_note;
  std::string hid_note;
  double price;
  int repair_state;
  std::string sn_imei;
  int cust_device_id;
};
