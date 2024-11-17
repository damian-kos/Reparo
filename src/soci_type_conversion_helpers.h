#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "models/simple_models.h"
#include "models/device.h"
#include "models/alias.h"
#include "models/customer.h"
#include <iostream>

class Database;


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
      }

      static void to_base(const Customer& model, values& v, indicator& ind) {
        v.set("id", model.id);
        v.set("phone", model.phone);
        v.set("name", model.name);
        v.set("surname", model.surname);
        v.set("email", model.email);

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

      try {
        // Get optional fields with defaults
        int type_id = 0;
        std::string type_name = "Unknown";
        int brand_id = 0;
        std::string brand_name = "Unknown";
        std::cout << "Column count: " << columns << std::endl;
        if (columns > 4) {
          type_id = v.get<int>("type_id");
          type_name = v.get<std::string>("type", "Unknown");
          brand_id = v.get<int>("brand_id");
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
      v.set("type_id", model.type.id); 
      v.set("brand_id", model.brand.id);

      ind = i_ok;
    }
  };

}


