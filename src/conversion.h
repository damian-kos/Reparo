#pragma once
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>




namespace Convert {
  // Forward declare instead of full implementation
  std::string TmToStr(const std::tm& _date);
  template <typename T>
  bool ContainsItemWithId(const std::vector<T>& vec, int id) {
    return std::any_of(vec.begin(), vec.end(), [id](const T& item) {
      return item.id == id;
      });
  }

  template <typename T>
  void PushBackIfUnique(std::vector<T>& vec, const T& item) {
    if (!ContainsItemWithId(vec, item.id)) {
      vec.push_back(item);
    }
  }

  inline std::string to_string_2dp(double val) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << val;
    return ss.str();
  }
}