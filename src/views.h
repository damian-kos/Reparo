#pragma once
#include <string>
#include <vector>
#include "models/customer.h"
#include "models/repair.h"
#include "models/parts.h"
#include "imgui.h"
#include "text_fields.h"
#include "filters.h"
#include "combo.h"
#include "base_window.h"
#include "invoices.h"

enum ViewStateFlags_ {
  ViewStateFlags_Default          = 1 << 1,
  ViewStateFlags_Select           = 1 << 2,
  ViewStateFlags_RemoveMultiple   = 1 << 3
};

typedef int ViewStateFlags;

class View : public RoWindow {
public:
  virtual void Render() = 0;
  virtual ~View() = default;

protected:
  struct Config {
    std::string window_id;
    int max_columns;
    // Column header name in SQL query table - Header name in imgui table
    std::vector<std::pair<std::string, std::string>> headers;
    bool is_window = true;
    ViewStateFlags flags = ViewStateFlags_Default;
  };
  Config config;
};

template <typename T>
class BaseTableView : public View {
public:
  BaseTableView(std::string _window_id, int _max_columns, 
    const std::vector<std::pair<std::string, std::string>>& _headers 
    , ViewStateFlags _flags = ViewStateFlags_Default, bool _is_window = true) {  
    config.window_id = _window_id;
    config.max_columns = _max_columns;
    config.headers = _headers;
    config.is_window = _is_window;
    config.flags = _flags;
    open = true;
  }

  void Render() override {
    if(config.is_window)
      ImGui::Begin(config.window_id.c_str(), &open);
    Filters();
    if (ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeY))
      if (ImGui::BeginTable(config.window_id.c_str(), config.max_columns, ImGuiTableFlags_RowBg  | ImGuiTableFlags_Borders | ImGuiTableFlags_Sortable | ImGuiTableFlags_Hideable  | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Resizable )) {
        for (const auto& header : config.headers) {
          ImGui::TableSetupColumn(header.second.c_str());
        }

        ImGui::TableHeadersRow();

        Sort();

        for (const auto& item : data) {
          ImGui::TableNextRow();
        
          DefaultRenderItem(item);
        
        }

        ImGui::EndTable();
      }
    ImGui::EndChild();

    if (config.is_window)
      ImGui::End();
  }


protected:
  virtual void DefaultRenderItem(const T& item) {
    // Implement a basic default rendering
    // This is a placeholder and should be specialized for different data types
  }

  virtual void ActionsOnTable(T& _item) {
    if (config.flags & ViewStateFlags_Default) {
      DefaultAction(_item);
    }
    else if (config.flags & ViewStateFlags_Select) {
      SelectAction(_item);
    }
    else if (config.flags & ViewStateFlags_RemoveMultiple) {
      RemoveMultipleAction(_item);
    }
  }
  virtual void DefaultAction(T& _item) {}
  virtual void SelectAction(T& _item) {}
  virtual void RemoveMultipleAction(T& _item) {}
  std::vector<T> data;
  virtual void LoadData(const std::string& _orderby = "", const int& _direction = 0);
  virtual void Sort();
  virtual void Filters();

};

class CustomerView : public BaseTableView<Customer> {
public:
  explicit CustomerView();
  ~CustomerView();

protected:
  void DefaultRenderItem(const Customer& customer) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;
  void DefaultAction(Customer& _customer) override;

};

class RepairView : public BaseTableView<Repair> {
public:
  explicit RepairView(); 

protected:
  void DefaultRenderItem(const Repair& _repair) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;
  void DefaultAction(Repair& _repair) override;
  void Filters() override;

private:
  PhoneField phone;
  QueriedTextField id_filter;
  DateFilter date;
  struct Timeline {
    std::string column;
    std::string name;
  };
  std::vector<Timeline> timelines;
  RoCombo<Timeline> timeline_combo;
  RoTabBar<RepairState> states;
  std::string state_query = "";
};

class InventoryView : public BaseTableView<Part> {
public:
  explicit InventoryView();
  explicit InventoryView(const std::string& _window_id, ViewStateFlags _flags);

  // SelectAction state
  RepairItem& GetSelectedItem();
private:
  void Init(const std::string& _window_id, ViewStateFlags _flags, const bool& _is_window = true);
  void DefaultRenderItem(const Part& _part) override;
  void DefaultAction(Part& _item) override;
  void SelectAction(Part& _item) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;
  void Filters() override;
  Part SelectState();
  ItemFilter item_filter;
  RepairItem repair_item;
};

class DevicesView : public BaseTableView<Device> {
public:
  explicit DevicesView();

private:
  void DefaultRenderItem(const Device& _device);
  void LoadData(const std::string& _orderby = "", const int& _direction = 0);
  void DefaultAction(Device& _device) override;
  void Filters();
  QueriedTextField device_filter;
  RoCombo<DeviceType> device_type_filter;
  RoCombo<Brand> brand_filter;
};

class PurchaseInvoiceView : public BaseTableView<PurchaseInvoice> {
public:
  explicit PurchaseInvoiceView();

private:
  void Init();

};

template<typename T>
inline void BaseTableView<T>::LoadData(const std::string & _orderby, const int& _direction) {
}

template<typename T>
inline void BaseTableView<T>::Sort() {
  if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs()) {
    if (sort_specs->SpecsDirty) {
      LoadData(config.headers[sort_specs->Specs->ColumnIndex].first,
        sort_specs->Specs->SortDirection);
      sort_specs->SpecsDirty = false;
    }
  }
}

template<typename T>
inline void BaseTableView<T>::Filters() {
}

