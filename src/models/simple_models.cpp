#include "simple_models.h"
#include "../../src/text_fields.h"
#include "../../src/database.h"

#include "../../src/modal.h"
#include "../../src/RoLocalization.h"
// We have these methods here because of the problem of circular dependency of header files

template<typename Derived>
const std::string SimpleModel<Derived>::ToString() const {
  return std::to_string(id) + " | " + name;
}

template<typename Derived>
const char* SimpleModel<Derived>::GetForCell() const {
  return name.empty() ? _("Unkown") : name.c_str();
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
void SimpleModel<Derived>::EditModal() {
  if (ImGui::Button(_("Edit record"))) {
    ModalConfig config;
    config.Title(_("Edit this record?"))
      .Msg(_("All the elements with this record will be changed as well."));
    SimpleModelModal modal(*this, config);
    StackModal::SetModal(modal);
  }
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

template<typename Derived>
void SimpleModel<Derived>::DeleteModal() {
  if (ImGui::Button("Delete")) {
    ModalConfig config;
    config.Title(_("Delete this record?"))
      .Msg(_("Are you sure? All records which are including this record will be set to Unknown."));
    SimpleModelModal modal(*this, config);
    StackModal::SetModal(modal);
  }
}

template<typename Derived>
bool SimpleModel<Derived>::Delete() {
  bool action = false;
  if (ImGui::Button("Confirm")) {
    action = true;
    DeleteInDb();
    ImGui::CloseCurrentPopup();
  }
  return action;
}

// Explicit instantiations for the types used
template struct SimpleModel<Brand>;
template struct SimpleModel<RepairState>;
template struct SimpleModel<Category>;
template struct SimpleModel<PaymentMethod>;
template struct SimpleModel<DeviceType>;
template struct SimpleModel<Color>;
template struct SimpleModel<Quality>;

double Price::ExVat() const {
  return amount / (1 + (vat_rate / 100));
}
