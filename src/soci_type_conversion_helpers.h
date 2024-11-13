#pragma once
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "models/simple_models.h"
#include <iostream>


// Concept definition
template<typename T>
concept IsSimple = std::is_convertible_v<T&, SimpleModel&>;

// Helper class that will contain our common implementation
template<IsSimple T>
struct simple_type_conversion_helper {
  static void from_base(soci::values const& v, soci::indicator ind, T& model) {
    std::cout << "Converting from base for " << typeid(T).name() << std::endl;
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