class Customer;
class Repair;

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
  int InsertRepair(Repair& repair);
  int InsertCustomDevice(Repair& repair);

  template <typename T>
  int InsertSimpleModel(T& _model);
}