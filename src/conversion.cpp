#include "conversion.h"
#include <sstream>
#include <iomanip>

namespace Convert {
  std::string TmToStr(const std::tm& _date) {
    std::stringstream ss;
    ss << std::put_time(&_date, "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }
}