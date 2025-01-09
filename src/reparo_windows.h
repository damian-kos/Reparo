#pragma once
#include <unordered_map>
#include "text_fields.h"
#include "models/simple_models.h"
#include "database.h"
#include "combo.h"
#include "attributes.h"
#include "models/customer.h"
#include "models/supplier.h"
#include "base_window.h"
#include "imguidatechooser.h"
#include "invoices.h"

class StackModal;

class CustomerWin : public RoWindow {
public:
  CustomerWin();
  CustomerWin(TFFlags phone_flags);
  ~CustomerWin();
  void Init();
  void Render();
  void Debug();
  void Feedback();
  void Submit();
  void FillBuffersByPhone(Customer& autofill);
  void InputFields();
  void Addresses();
  void FieldsValidate();
  Customer CreateCustomer();
  Customer GetEntity();
  bool error = true;
private:
  PhoneField phone;
  NameField name;
  SurnameField surname;
  EmailField email;
  std::vector<TextField> billing_address;
  std::vector<TextField> ship_address;

  //bool open = true;
};

class DeviceWin : public RoWindow {
public:
  DeviceWin();
  DeviceWin(CustomDevice _custom);
  ~DeviceWin();
  void Init();
  void Render();
  void DeviceName();
  void FillDeviceByName(Device& autofill);

private:
  DeviceField name;
  RoCombo<Brand> brand_combo;
  RoCombo<DeviceType> type_combo;
  Attributes<Color> colors;
  Attributes<Alias> aliases;
};

class RepairWin : public RoWindow {
public:
  RepairWin();
  void Init();
  void Render();
  void CustomerSection();
  void DeviceSection();
  void NotesSection();
  void FieldsValidate();
  void DeviceFeedback();
  void NotesFeedback();
  void PriceSection();
  void PriceFeedback();
  void Submit();
  void RepairValidated();
  Device CreateDevice();

private:
  CustomerWin customer_section;
  bool device_section_error = true;
  bool notes_section_error = true;
  bool price_section_error = true;
  bool error = true;
  double price = 0;
  bool price_can_be_zero;
  DeviceField device;
  SimpleModelField<Category> category;
  RelationalField<Color, DeviceField> color;
  TextField sn_imei;
  TextField vis_note;
  TextField hid_note;
};

class  PartsWin : public RoWindow {
  public:
    PartsWin();
    void Init();
    void Render();
    void Feedback();
    void PriceSection(const std::string& _action, Price& _price);
    void QuantitySection();
    void CompatibleTablePicker();
    void CompatibleEntriesBox();
    template <typename T>
    void ListEntriesInBox(float& last_btn, float window, std::unordered_map<int, T>& entries);
    void Submit();
    void Filters();
    void LoadDevices();
    void FieldsValidate();
  private:
    // Main insert input fields
    SimpleModelField<Supplier> supplier;
    OwnSKUField own_sku_field;
    QueriedTextField name_field;
    SimpleModelField<Color> color;
    RoCombo<Quality> quality;
    RoCombo<Category> category;
    QueriedTextField location;
    Price purch_price;
    Price sell_price;
    int quantity = 0;
    // Filter field
    QueriedTextField device_filter;
    SimpleModelField<Brand> brand_filter;
    SimpleModelField<DeviceType> device_type_filter;
    std::vector<Device> devices;
    std::unordered_map<int, Device> cmptble_devices;
    std::unordered_map<int, Alias> cmptble_aliases;
    bool error = true;
};

class CustomDeviceWin : public RoWindow {
public:
  CustomDeviceWin();
  void Init();
  void Render();

  bool error = true;
private:
  std::vector<CustomDevice> devices;
};

class PurchaseInvoiceWin : public RoWindow {
public:
  PurchaseInvoiceWin();
  void Init();
  void Render();

  void RenderInvoiceHeader();
  void RenderInvoiceNumber();
  void RenderDateFields();
  void RenderSupplierField();
  void AddSupplierBtn();
  void RenderInvoiceItems();
  void RenderInvoiceTableHeaders();
  void RenderInvoiceTableRows();
  void RenderAddItemButton();
  void Submit();

private:
  tm purchase_date;
  tm arrival_date;
  tm create_date;
  SimpleModelField<Supplier> supplier_field;
  Supplier supplier;
  std::vector<InvoiceItem> parts;
};

class SupplierWin : public RoWindow {
public:
  SupplierWin();
  void Init();
  void Render();
  void Feedback();
  void InputFields();
  Supplier GetEntity();

private:
  void Submit();
  void FieldsValidate();
  TextField name;
  std::vector<TextField> address;
  bool error;
};
template <typename T>
class SimpleModelWin : public RoWindow{
public:
  SimpleModelWin();
  void Render();
  void LoadData();

private:
  T model;
  TextField name;
  std::vector<T> values;
};

template<typename T>
inline SimpleModelWin<T>::SimpleModelWin() {
  open = true;
  LoadData();
}

template<typename T>
inline void SimpleModelWin<T>::LoadData() {
  std::string text = Database::Select<T>().From().GetSql();
  std::cout << text << std::endl;
  values = Database::Select<T>().From().All();
}