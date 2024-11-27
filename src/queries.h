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

  int UpdateBillingAddress(Customer& customer) {
    Customer c = customer;
    int billing_addr_id = c.billing_addresses.Get().ID();
    if (!c.billing_addresses.Get().Lines().empty()) {
      const auto& billing = c.billing_addresses.Get().Lines(); // Using first address
      Database::sql << "UPDATE billing_addresses "
        "SET line1 = :l1, line2 = :l2, line3 = :l3 , line4 = :l4, line5 = :l5 "
        "WHERE id = :id "
        "RETURNING id",
        soci::use(billing[0]),
        soci::use(billing[1]),
        soci::use(billing[2]),
        soci::use(billing[3]),
        soci::use(billing[4]),
        soci::use(billing_addr_id);
    }
    return billing_addr_id;
  }

  int InsertShippingAddress(Customer& customer) {
    Customer c = customer;
    int ship_addr_id = 0;
    if (!c.ship_addresses.Get().Lines().empty()) {
      const auto& ship = c.ship_addresses.Get().Lines(); // Using first address
      Database::sql << "INSERT INTO ship_addresses "
        "(line1, line2, line3, line4, line5) "
        "VALUES (:l1, :l2, :l3, :l4, :l5) "
        "RETURNING id",
        soci::use(ship[0]),
        soci::use(ship[1]),
        soci::use(ship[2]),
        soci::use(ship[3]),
        soci::use(ship[4]),
        soci::into(ship_addr_id);
    }
    customer.ship_addresses.SetID(ship_addr_id);
    return ship_addr_id;
  }

  int UpdateShippingAddress(Customer& customer) {
    Customer c = customer;
    int ship_addr_id = c.ship_addresses.Get().ID();
    if (!c.ship_addresses.Get().Lines().empty()) {
      const auto& ship = c.ship_addresses.Get().Lines(); // Using first address
      Database::sql << "UPDATE ship_addresses "
        "SET line1 = :l1, line2 = :l2, line3 = :l3 , line4 = :l4, line5 = :l5 "
        "WHERE id = :id "
        "RETURNING id",
        soci::use(ship[0]),
        soci::use(ship[1]),
        soci::use(ship[2]),
        soci::use(ship[3]),
        soci::use(ship[4]),
        soci::use(ship_addr_id);
    }
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

  int UpdateCustomer(Customer& customer) {
    Customer c = customer;
    int customer_id = c.id;
    Database::sql << "UPDATE customers "
      "SET phone = :phone, name = :name, surname = :surname, email = :email, billing_addr_id = :billing_addr_id, ship_addr_id = :ship_addr_id "
      "WHERE id = :id "
      "RETURNING id",
      soci::use(c);
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