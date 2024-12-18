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

class View {
public:
  virtual void Render() = 0;
  virtual ~View() = default;

protected:
  struct Config {
    std::string window_id;
    int max_columns;
    // Column header name in SQL query table - Header name in imgui table
    std::vector<std::pair<std::string, std::string>> headers;
  };
  Config config;
};

template <typename T>
class BaseTableView : public View {
public:
  BaseTableView(std::string _window_id, int _max_columns, const std::vector<std::pair<std::string, std::string>>& _headers,
    std::vector<T> data) {  
    config.window_id = _window_id;
    config.max_columns = _max_columns;
    config.headers = _headers;
    this->data = std::move(data);  
  }


  void Render() override {
    ImGui::Begin(config.window_id.c_str());
    Filters();
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
    ImGui::End();
  }


protected:
  virtual void DefaultRenderItem(const T& item) {
    // Implement a basic default rendering
    // This is a placeholder and should be specialized for different data types
  }
  std::vector<T> data;
  virtual void LoadData(const std::string& _orderby = "", const int& _direction = 0);
  virtual void Sort();
  virtual void Filters();

};

class CustomerView : public BaseTableView<Customer> {
public:
  CustomerView(std::vector<Customer> customers)
    : BaseTableView<Customer>(
      "Customers view",
      5, 
      { 
        {"id", "ID"},
        {"phone", "Phone"},
        {"name", "Name"},
        {"surname", "Surname"},
        {"email", "Email"}
      },
      std::move(customers)
    ) { }

protected:
  void DefaultRenderItem(const Customer& customer) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;

};

class RepairView : public BaseTableView<Repair> {
public:
  explicit RepairView(std::vector<Repair> _repairs); 

protected:
  void DefaultRenderItem(const Repair& _repair) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;
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
  explicit InventoryView(std::vector<Part> _parts);

private:
  void DefaultRenderItem(const Part& _part) override;
  void LoadData(const std::string& _orderby = "", const int& _direction = 0) override;
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

