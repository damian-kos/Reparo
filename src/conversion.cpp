#include "conversion.h"
#include <sstream>
#include <iomanip>

namespace Convert {
  std::string TmToStr(const std::tm& _date) {
    // Check if the year is 1970 (tm_year is the number of years since 1900)
    if (_date.tm_year == 0) {
      return "Ongoing";
    }

    // Otherwise, format and return the date as a string
    std::stringstream ss;
    ss << std::put_time(&_date, "%Y-%m-%d %H:%M:%S");
    return ss.str();
  }
}