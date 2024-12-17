#pragma once
#include <unordered_map>
#include <string>

class DateFilter {
public:
  DateFilter();
  bool Render();
  std::string GetForSQL();
private:
  tm from;
  tm to;
  std::unordered_map<int, std::string> directions;
  int selected = 1;
  bool disabled = true;
};