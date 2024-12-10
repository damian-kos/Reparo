#pragma once
#include "customer.h"
#include "simple_models.h"
#include "device.h"

class Repair  {
public:
  int id;
  Customer customer;
  Device device;
  Category category;
  Color color;
  std::string sn_imei;
  std::string vis_note;
  std::string hid_note;
  double price;
  RepairState repair_state;
  int cust_device_id;
  static constexpr std::string_view table = "repairs";
  const std::string ToString() const;
  void View();
  void InsertModal();
  void InsertToDb();
};
