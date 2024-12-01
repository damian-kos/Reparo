#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "models/simple_models.h"
#include "models/alias.h"
#include "models/customer.h"
#include "models/device.h"
#include "models/repair.h"
#include "models/supplier.h"
#include "models/parts.h"
#include "conversion.h"
#include <iostream>


// Concept definition
template<typename T>
concept IsSimple = std::is_convertible_v<T&, SimpleModel<T>&>;

// Helper class that will contain our common implementation
template<IsSimple T>
struct simple_type_conversion_helper {
  static void from_base(soci::values const& v, soci::indicator ind, T& model) {
    model.id = v.get<int>("id");
    model.name = v.get<std::string>(model.column);
  }

  static void to_base(const T& model, soci::values& v, soci::indicator& ind) {
    v.set("id", model.id);
    v.set("name", model.name);
    ind = soci::i_ok;
  }
};

namespace soci {
  template<typename T>
    requires IsSimple<T>
  struct type_conversion<T> {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, T& model) {
      simple_type_conversion_helper<T>::from_base(v, ind, model);
    }

    static void to_base(const T& model, values& v, indicator& ind) {
      simple_type_conversion_helper<T>::to_base(model, v, ind);
    }
  };

  template <>
    struct type_conversion<Alias> {
      typedef values base_type;

      static void from_base(const values& v, indicator ind, Alias& model) {
        if (ind == i_null) {
          throw std::runtime_error("Null value fetched from database");
        }
        model.id = v.get<int>("id");
        model.name = v.get<std::string>(model.column);
      }

      static void to_base(const Alias& model, values& v, indicator& ind) {
        v.set("id", model.id);
        v.set("name", model.name);
        ind = i_ok;
      }
   };

  template <>
  struct type_conversion<Customer> {
    typedef values base_type;

    static void from_base(const values& v, indicator ind, Customer& model) {
      std::cout << "Converting from base for " << typeid(Customer).name() << std::endl;

      if (ind == i_null) {
        throw std::runtime_error("Null value fetched from database");
      }
      model.id = v.get<int>("id");
      model.phone = v.get<std::string>("phone");
      model.name = v.get<std::string>("name");
      model.surname = v.get<std::string>("surname");
      model.email = v.get<std::string>("email");      
      int billing_addr_id = v.get<int>("billing_addr_id", -1);
      model.billing_addresses.SetID(billing_addr_id);
      int ship_addr_id = v.get<int>("ship_addr_id", -1);
      model.ship_addresses.SetID(ship_addr_id);

    }

    static void to_base(const Customer& model, values& v, indicator& ind) {
      v.set("id", model.id);
      v.set("phone", model.phone);
      v.set("name", model.name);
      v.set("surname", model.surname);
      v.set("email", model.email);
      v.set("billing_addr_id", model.billing_addresses.Get().ID());
      v.set("ship_addr_id", model.ship_addresses.Get().ID());


      ind = i_ok;
    }
  };

  template <>
  struct type_conversion<Device>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Device& model) {
      int columns = v.get_number_of_columns();
      if (columns <= 0) { return; }
      std::cout << "Converting from base for " << typeid(Device).name() << std::endl;
        std::cout << "Column count: " << columns << std::endl;

      try {
        // Get optional fields with defaults
        int type_id  = v.get<int>("type_id", -1);
        std::string type_name = "Unknown";
        int brand_id = v.get<int>("brand_id", -1);
        std::string brand_name = "Unknown";
        if (columns > 4) {
          type_name = v.get<std::string>("type", "Unknown");
          brand_name = v.get<std::string>("brand", "Unknown");
        }

        DeviceType type{ type_id, type_name };
        Brand brand{ brand_id, brand_name };

        // Build the device object
        model.id = v.get<int>("id");
        model.name = v.get<std::string>("model");
        model.brand = brand;
        model.type = type;

      }
      catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in Device conversion: ") + e.what());
      }
    }

    static void to_base(const Device& model, values& v, indicator& ind) {
      if (model.name.empty()) {
        throw std::runtime_error("Device model name cannot be empty");
      }
      
      v.set("model", model.name);
      v.set("type_id", model.type.id, model.brand.id == -1 ? i_null : i_ok); 
      v.set("brand_id", model.brand.id, model.brand.id == -1 ? i_null : i_ok);

      ind = i_ok;
    }
  };

  template <>
  struct type_conversion<Repair> {
    typedef values base_type;

    static void from_base(const values& v, indicator ind, Repair& model) {
      std::cout << "Converting from base for " << typeid(Repair).name() << std::endl;

      if (ind == i_null) {
        throw std::runtime_error("Null value fetched from database");
      }
      model.id = v.get<int>("id");
      model.customer.id = v.get<int>("customer_id");
      //model.customer = Database::Get().Customer_(model.customer.id);
      model.device.id = v.get<int>("model_id", -1);
      //model.device = Database::Get().Device_(model.device.id);
      model.color.id = v.get<int>("color_id", -1);
      //model.color = Database::Get().SimpleModel_<int, Color>(model.color.id);
      model.vis_note = v.get<std::string>("visible_desc");
      model.hid_note = v.get<std::string>("hidden_desc");
      model.price = v.get<double>("price");
      model.repair_state.id = v.get<int>("repair_state_id");
      //model.repair_state = Database::Get().SimpleModel_<int, RepairState>(model.repair_state.id);
      model.sn_imei = v.get<std::string>("sn_imei");
      model.cust_device_id = v.get<int>("cust_device_id", -1);

    }

    static void to_base(const Repair& model, values& v, indicator& ind) {
      v.set("id", model.id);
      v.set("customer_id", model.customer.id);
      v.set("model_id", model.device.id, model.device.id == -1 ? i_null : i_ok);
      v.set("category_id", model.category.id, model.category.id == -1 ? i_null : i_ok);
      v.set("color_id", model.color.id, model.color.id == -1 ? i_null : i_ok);
      v.set("visible_desc", model.vis_note);
      v.set("hidden_desc", model.hid_note);
      v.set("price", model.price);
      v.set("repair_state_id", model.repair_state.id);
      v.set("sn_imei", model.sn_imei);
      v.set("cust_device_id", model.cust_device_id, model.cust_device_id == -1 ? i_null : i_ok);

      ind = i_ok;
    }
  };

  template <>
  struct type_conversion<Supplier> {
    typedef values base_type;

    static void from_base(const values& v, indicator ind, Supplier& model) {
      std::cout << "Converting from base for " << typeid(Supplier).name() << std::endl;

      if (ind == i_null) {
        throw std::runtime_error("Null value fetched from database");
      }
      model.id = v.get<int>("id");
      model.name = v.get<std::string>("supplier");
      std::vector<std::string> vec(5);
      for (int i = 1; i < 6; ++i) {
        std::string str = "line" + std::to_string(i);
        vec.push_back(v.get<std::string>(str, ""));
      }
      model.address.SetLines(vec);


    }

    static void to_base(const Supplier& model, values& v, indicator& ind) {
      v.set("id", model.id);
      v.set("supplier", model.name);
      std::vector<std::string> vec = model.address.Get().Lines();
      for (int i = 0; i < 5; ++i) {
        std::string str = "line" + std::to_string(i+1);
        v.set(str, vec[i]);
      }

      ind = i_ok;
    }
  };

  template <>
  struct type_conversion<Part> {
    typedef values base_type;

    static void from_base(const values& v, indicator ind, Part& model) {
      std::cout << "Converting from base for " << typeid(Part).name() << std::endl;

      if (ind == i_null) {
        throw std::runtime_error("Null value fetched from database");
      }
      model.id = v.get<int>("id");
      model.name = v.get<std::string>("name");
      model.own_sku = v.get<std::string>("own_sku");
      model.quality.id = v.get<int>("quality_id", -1);
      model.sell_price = v.get<double>("sell_price", 0);
      model.sell_price_ex_vat = v.get<double>("sell_price_ex_vat", 0.0);
      model.color.id = v.get<int>("color_id", -1);
      model.quantity = v.get<int>("quantity", 0);
      model.purch_price = v.get<double>("purch_price", 0);
      model.purch_price_ex_vat = v.get<double>("purch_price_ex_vat", 0);
      model.location = v.get<std::string>("location", "N/A");
      model.reserved_quantity = v.get<int>("reserved_quantity", 0);
      model.created_at = v.get<std::tm>("created_at");
      model.updated_at = v.get<std::tm>("updated_at");
      std::cout << Convert::TmToStr(model.updated_at) << std::endl;

      // If Part retreived with joined tables
      if (v.get_number_of_columns() > 14) {
        model.color.name = v.get<std::string>("color", "Unknown");
        model.quality.name = v.get<std::string>("quality", "Unknown");
        
      }
    }

    static void to_base(const Part& model, values& v, indicator& ind) {
      v.set("id", model.id);
      v.set("name", model.name);
      v.set("own_sku", model.own_sku);
      v.set("quality_id", model.quality.id);
      v.set("sell_price", model.sell_price);
      v.set("sell_price_ex_vat", model.sell_price_ex_vat);
      v.set("color_id", model.quality.id);
      v.set("quantity", model.quantity);
      v.set("purch_price", model.purch_price);
      v.set("purch_price_ex_vat", model.purch_price_ex_vat);
      v.set("location", model.location);
      v.set("reserved_quantity", model.reserved_quantity);
      v.set("created_at", model.created_at);
      v.set("updated_at", model.updated_at);

      ind = i_ok;
    }
  };
}


