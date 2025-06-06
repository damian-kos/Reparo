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
#include "views.h"
#include "buttons.h"


class StackModal;

class CustomerWin : public RoWindow {
public:
  CustomerWin();
  CustomerWin(TFFlags phone_flags);
  CustomerWin(Customer& _customer);
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
  bool GetError() const;

  // WindowState_Update
  Customer& GetPrevious();

private:
  PhoneField phone;
  NameField name;
  SurnameField surname;
  EmailField email;
  std::vector<TextField> billing_address;
  std::vector<TextField> ship_address;

  // WindowState_Insert
  void RenderInsertState();

  // WindowState_Update
  void RenderUpdateState();
  Customer previous_customer;
  bool error = true;
};

class DeviceWin : public RoWindow {
public:
  DeviceWin();
  DeviceWin(Device _device);
  DeviceWin(CustomDevice _custom);
  ~DeviceWin();
  void Init();
  void Render();
  bool Submit();
  void DeviceName();
  void FillDeviceByName(Device& autofill);
  Device CreateDevice();

private:
  DeviceField name;
  RoCombo<Brand> brand_combo;
  RoCombo<DeviceType> type_combo;
  Attributes<Color> colors;
  Attributes<Alias> aliases;

  // Shared between states
  void RenderSharedState();

  // WindowState_Insert
  void RenderInsertState();
  void SubmitInsert();

  // WindowState_Update
  void RenderUpdateState();
  void SubmitUpdate();
  Device previous_device;
};

class RepairWin : public RoWindow {
public:
  RepairWin();
  RepairWin(Repair& _repair);
  void Init();
  void Render();
  void UpdatesSection();
  void ItemAssign();
  void RenderAssignedItems();
  void CustomerSection();
  void DeviceSection();
  void NotesSection();
  void PriceSection();
  void FieldsValidate();
  void DeviceFeedback();
  void NotesFeedback();
  void PriceFeedback();
  void Submit();
  Repair CreateRepair();
  void RepairValidated();
  Device CreateDevice();

  // WindowState_Insert
  void RenderInsertState();

  // WindowState_Update
  void RenderUpdateState();
  Repair& GetPrevious();

private:
  CustomerWin customer_section;
  bool device_section_error = true;
  bool notes_section_error = true;
  bool price_section_error = true;
  bool price_section_warning = true;
  bool error = true;
  double price = 0;
  bool price_can_be_zero;
  DeviceField device;
  SimpleModelField<Category> category;
  RelationalField<Color, DeviceField> color;
  TextField sn_imei;
  TextField vis_note;
  TextField hid_note;
  ItemsContainer<RepairItem> items;

  // WindowState_Insert
  void Insert(Repair _repair) const;

  // WindowState_Update
  void StateSection();
  void Update(Repair _repair) const;
  void CompareRepairs();
  Repair previous_repair;
  RoCombo<RepairState> repair_state;
  std::vector<RepairUpdate> updates_history;

};

class  PartsWin : public RoWindow {
  public:
    PartsWin();
    PartsWin(Part& _part);
    void Render();
    bool IsSubmitPressed();

    Part CreatePart();

    // WindowState_Update
    Part GetPrevious();

private:
    void Init();
    void FieldsValidate();
    void RenderSharedBetweenStates();
    void LoadDevices();
    void Filters();
    template <typename T>
    void ListEntriesInBox(float& last_btn, float window, std::unordered_map<int, T>& entries);
    template <typename T>
    void ListEntriesInBox(float& last_btn, float window, std::vector<T>& entries);
    void Feedback();
    void PriceSection(const std::string& _action, Price& _price);
    void QuantitySection();
    void CompatibleTablePicker();
    void CompatibleEntriesBox();

    // WindowState_Insert
    void RenderInsertState();
    void Insert(Part _part) const;
    void SubmitInsert();

    // WindowState_Update
    void RenderUpdateState();
    void SubmitUpdate();

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
    std::vector<Device> device_entries;
    std::vector<Alias> alias_entries;
    Buttons submit;
    bool error = true;

    Part previous_part;
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
  PurchaseInvoiceWin(PurchaseInvoice& _invoice);

  void Init();
  void Render();
  bool IsSubmitPressed();

  PurchaseInvoice CreatePurchaseInvoice();
  void ResetFields();

private:
  void RenderSharedBetweenStates();
  void RenderInvoiceHeader();
  void RenderInvoiceNumber();
  void RenderDateFields();
  void RenderSupplierField();
  void AddSupplierBtn();
  void RenderInvoiceItems();
  void RenderInvoiceTableHeaders();
  void RenderInvoiceTableRows();
  void RenderAddItemButton();

  void Feedback();
  void FieldsValidate();
  SimpleModelField<PurchaseInvoice> invoice_number;
  QueriedTextField external_id;
  RoDate purchase_date;
  RoDate arrival_date;
  RoDate create_date;
  SimpleModelField<Supplier> supplier_field;
  Supplier supplier;
  ItemsContainer<InvoiceItem> invoice_items;
  Buttons submit;
  bool error = true;

  // WidowsState_Insert
  void RenderInsertState();
  void Insert(PurchaseInvoice _invoice) const;
  void SubmitInsert();

  // WindowState_Update
  void RenderUpdateState();
  void SubmitUpdate();
  PurchaseInvoice previous_invoice;
};

class SupplierWin : public RoWindow {
public:
  SupplierWin();
  SupplierWin(Supplier& _supplier);
  void Render();
  void RenderSharedBetweenStates();
  void RenderInsertState();
  void RenderUpdateState();

  void Feedback();
  void InputFields();
  Supplier GetEntity();
  Supplier CreateSupplier();
  void Clear();
  bool IsSubmitPressed();
  Supplier GetPrevious();

private:
  void Init();
  void FieldsValidate();
  void Submit();

  TextField name;
  std::vector<TextField> address;
  bool error = true;
  Buttons submit;

  // WindowState_Insert
  void SubmitInsert();
  // WindowState_Update
  void SubmitUpdate();
  Supplier previous_supplier;
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