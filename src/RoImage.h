#pragma once

#include <string>
#include <vector>

struct TextMetrics;

namespace RoImage {
  std::vector<std::string> GetWrappedLines(const int& width, const std::string& text, TextMetrics& metrics);
  void Image();
  void RepairImage();
  void TableTopBorder(int& _table_top_y, int _current_y, TextMetrics& _metrics);
  void SaleInvoiceImage();
}