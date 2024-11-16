#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "models/simple_models.h"
#include "models/device.h"
#include "models/alias.h"
#include <iostream>

class Database;

// Concept definition
template<typename T>
concept IsSimple = std::is_convertible_v<T&, SimpleModel&>;

// Helper class that will contain our common implementation
template<IsSimple T>
struct simple_type_conversion_helper {
  static void from_base(soci::values const& v, soci::indicator ind, T& model) {
    //std::cout << "Converting from base for " << typeid(T).name() << std::endl;
    //std::string column_ = column<T>::value;
    model.Set<T>()
      .ID(v.get<int>("id"))
      .Name(v.get<std::string>(model.Get<T>().Column()));
  }

  static void to_base(const T& model, soci::values& v, soci::indicator& ind) {
    v.set("id", model.Get<T>().ID());
    v.set("name", model.Get<T>().Name());
    ind = soci::i_ok;
  }
};

// Specialization of SOCI's type_conversion for IsSimple types
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
}

// Helper class that will contain our common implementation
namespace soci {
  template <>
  struct type_conversion<Device>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Device& model) {
        int columns = v.get_number_of_columns();
        if (columns <= 0) { return; }
      std::cout << "Converting from base for " << typeid(Device).name() << std::endl;

      try {
        auto build = model.Set<Device>();
 
        // Get optional fields with defaults
        int type_id = 0;
        std::string type_name = "Unknown";
        int brand_id = 0;
        std::string brand_name = "Unknown";
        std::cout << "Column count: " << columns << std::endl;
        if (columns > 4) {
          type_id = v.get<int>("type_id");
          type_name = v.get<std::string>("Type", "Unknown");
          brand_id = v.get<int>("brand_id");
          brand_name = v.get<std::string>("Brand", "Unknown");
        }

        DeviceType type(type_id, type_name);
        Brand brand(brand_id, brand_name);

        // Build the device object
        build.ID(v.get<int>("id"))
          .Model(v.get<std::string>("model"))
          .Type(type)
          .Brand_(brand);

      }
      catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error in Device conversion: ") + e.what());
      }
    }

    static void to_base(const Device& model, values& v, indicator& ind) {
      auto device = model.Get<Device>();

      std::string model_name = device.Name();

      std::cout << "Debug - to_base conversion:" << std::endl;
      std::cout << "Model name length: " << model_name.length() << std::endl;
      std::cout << "Model name: '" << model_name << "'" << std::endl;

      if (model_name.empty()) {
        throw std::runtime_error("Device model name cannot be empty");
      }

      
      v.set("model", model_name);
      v.set("type_id", device.Type().Get<DeviceType>().ID()); 
      v.set("brand_id", device.Brand_().Get<Brand>().ID());


      ind = i_ok;
    }
  };

  template <>
  struct type_conversion<Alias>
  {
    typedef values base_type;

    static void from_base(values const& v, indicator ind, Alias& model) {
      std::cout << "Converting from base for " << typeid(Alias).name() << std::endl;
      model.Set<Alias>()
        .ID(v.get<int>("id"))
        .Name(v.get<std::string>("alias"))
        .LinkID(v.get<int>("model_id"));

    }

    static void to_base(const Alias& model, values& v, indicator& ind) {
      const auto& data = model.Get<Alias>();
      v.set("id", data.ID());
      v.set("alias", data.Name());
      v.set("link_id", data.LinkID());

      ind = i_ok;
    }
  };
}