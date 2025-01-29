class Part;
class Customer;
class Repair;
class Supplier;
class PurchaseInvoice;
struct InvoiceItem;
struct RepairItem;
/// <summary>
/// Used to allow to run multiple queries in one Transaction without need to reopening database.
/// </summary>
namespace Query {
  int InsertBillingAddress(Customer& customer);
  int UpdateBillingAddress(Customer& customer);
  int InsertShippingAddress(Customer& customer);
  int UpdateShippingAddress(Customer& customer);
  int InsertCustomer(Customer& customer);
  int UpdateCustomer(Customer& customer);
  int InsertItem(Part& _part);
  int UpdateItem(Part& _part);
  int UpdateItem(InvoiceItem& _item);
  int UpdateRepair(Repair& repair);
  int UpdateRepairPart(RepairItem& _item);
  void InsertItemDevices(Part& _part);
  void InsertItemAliases(Part& _part);
  int InsertRepair(Repair& repair);
  int InsertCustomDevice(Repair& repair);
  void InsertRepairPart(RepairItem& _item);
  int InsertSupplier(Supplier& _supplier);
  int InsertPurchaseInvoice(PurchaseInvoice& _invoice);
  int InsertInvoiceItem(InvoiceItem& _item);

  template <typename T>
  int InsertSimpleModel(T& _model);
}