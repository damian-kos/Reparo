#include "simple_models.h"
#include "../../src/text_fields.h"
#include "../../src/database.h"

// We have these methods here because of the problem of circular dependency of header files

template<typename Derived>
const std::string SimpleModel<Derived>::ToString() const {
  return std::to_string(id) + " | " + name;
}

template <typename Derived>
void SimpleModel<Derived>::UpdateInDb() {
  Database::Update().OfSimpleModel(static_cast<const Derived&>(*this));
}

template <typename Derived>
void SimpleModel<Derived>::DeleteInDb() {
  Database::Delete().OfSimpleModel(static_cast<const Derived&>(*this));
}

template<typename Derived>
bool SimpleModel<Derived>::Edit() {
  bool action = false;
  static TextField field;
  field.Render();
  if (ImGui::Button("Confirm")) {
    action = true;
    name = field.Get();
    UpdateInDb();
    ImGui::CloseCurrentPopup();
  }
  return action;
}

// Explicit instantiations for the types used
template struct SimpleModel<Brand>;
template struct SimpleModel<RepairState>;
template struct SimpleModel<RepairCategory>;
template struct SimpleModel<PaymentMethod>;
template struct SimpleModel<DeviceType>;
template struct SimpleModel<Color>;

double Price::ExVat() {
  return amount / (1 + (vat_rate / 100));
}
