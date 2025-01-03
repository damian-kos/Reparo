#pragma once
#include <map>
#include "reparo_windows.h"
#include "views.h"

class WindowFactory {
public:
  static void AddWindow(const std::string& _window);
  static void Render();

private:
  static std::map<std::string, std::unique_ptr<RoWindow>> windows;
};