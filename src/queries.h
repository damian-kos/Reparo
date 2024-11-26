#pragma once
#include "database.h"
#include "../../src/models/customer.h"

namespace Query {
  int InsertBillingAddress(Customer& customer) {
    Customer c = customer;
    int billing_addr_id = 0;
    if (!c.billing_addresses.Get().Lines().empty()) {
      const auto& billing = c.billing_addresses.Get().Lines(); // Using first address
      Database::sql << "INSERT INTO billing_addresses "
        "(line1, line2, line3, line4, line5) "
        "VALUES (:l1, :l2, :l3, :l4, :l5) "
        "RETURNING id",
        soci::use(billing[0]),
        soci::use(billing[1]),
        soci::use(billing[2]),
        soci::use(billing[3]),
        soci::use(billing[4]),
        soci::into(billing_addr_id);
    }
    customer.billing_addresses.SetID(billing_addr_id);
    return billing_addr_id;
  }

  int InsertShippingAddress(Customer& customer) {
    Customer c = customer;
    int ship_addr_id = 0;
    if (!c.ship_addresses.Get().Lines().empty()) {
      const auto& shipping = c.ship_addresses.Get().Lines(); // Using first address
      Database::sql << "INSERT INTO ship_addresses "
        "(line1, line2, line3, line4, line5) "
        "VALUES (:l1, :l2, :l3, :l4, :l5) "
        "RETURNING id",
        soci::use(shipping[0]),
        soci::use(shipping[1]),
        soci::use(shipping[2]),
        soci::use(shipping[3]),
        soci::use(shipping[4]),
        soci::into(ship_addr_id);
    }
    customer.ship_addresses.SetID(ship_addr_id);
    return ship_addr_id;
  }

  int InsertCustomer(Customer& customer) {
    Customer c = customer;
    int customer_id = -1;
    Database::sql << "INSERT INTO customers "
      "(phone, name, surname, email, billing_addr_id, ship_addr_id) "
      "VALUES (:phone, :name, :surname, :email, :billing_addr_id, :ship_addr_id) "
      "RETURNING id",
      soci::use(c),
      soci::into(customer_id);
    customer.id = customer_id;
    return customer_id;
  }

  int InsertRepair(Repair& repair) {
    Repair r = repair;
    int repair_id = 0;
    Database::sql << "INSERT INTO repairs (customer_id, model_id, category_id, color_id, visible_desc, hidden_desc, price, repair_state_id, sn_imei, cust_device_id) "
      "VALUES (:customer_id, :model_id, :category_id, :color_id, :visible_desc, :hidden_desc, :price, :repair_state_id, :sn_imei, :cust_device_id) "
      "RETURNING id",
      soci::use(r),
      soci::into(repair_id);
    repair.id = repair_id;
    return repair_id;
  }
}