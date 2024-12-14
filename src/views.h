#pragma once
#include <string>
#include <vector>
#include "models/customer.h"
#include "models/repair.h"
#include "imgui.h"

class View {
public:
  virtual void Render() = 0;
  virtual ~View() = default;

protected:
  struct Config {
    std::string window_id;
    int max_columns;
    std::vector<std::string> headers;
  };
  Config config;
};

template <typename T>
class BaseTableView : public View {
public:
  BaseTableView(std::string _window_id, int _max_columns, const std::vector<std::string>& _headers,
    std::vector<T> data) {  
    config.window_id = _window_id;
    config.max_columns = _max_columns;
    config.headers = _headers;
    this->data = std::move(data);  
  }


  void Render() override {
    ImGui::Begin(config.window_id.c_str());
    if (ImGui::BeginTable(config.window_id.c_str(), config.max_columns, ImGuiTableFlags_RowBg  | ImGuiTableFlags_Borders)) {
      for (const auto& header : config.headers) {
        ImGui::TableSetupColumn(header.c_str());
      }
      ImGui::TableHeadersRow();

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

};

class CustomerView : public BaseTableView<Customer> {
public:
  CustomerView(std::vector<Customer> customers)
    : BaseTableView<Customer>(
      "Customers view",
      5, 
      { "ID", "Phone", "Surname", "Name", "Email"},
      std::move(customers)
    ) { }

protected:
  void DefaultRenderItem(const Customer& customer) override;

};

class RepairView : public BaseTableView<Repair> {
public:
  RepairView(std::vector<Repair> _repairs)
    : BaseTableView<Repair>(
      "Repairs view",
      10,
      { "ID", "Cust. Phone", "Cust. Name", "Device", "Category", "Notes", "Hidden note", "Price", "State", "SN / IMEI"},
      std::move(_repairs)
      ) { }

protected:
  void DefaultRenderItem(const Repair& _repair) override;

};