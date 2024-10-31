#pragma once

#include <string>
#include <vector>

struct TextMetrics;

namespace RoImage {
  std::vector<std::string> GetWrappedLines(const int& width, const std::string& text, TextMetrics& metrics);
  void Image();
}