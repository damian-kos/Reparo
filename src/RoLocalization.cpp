// main.cpp
#include "RoLocalization.h"
#include "imgui.h"
#include <string>
#include <vector>
#include <iostream>



namespace Loc {
void initLocalization() {
  #ifdef _WIN32
  const char* localePath = "locale";  // Relative path to .mo files
  #else
  const char* localePath = "/usr/share/locale";  // Standard Unix path
  #endif

  auto& loc = Localization::getInstance();
  if (!loc.init("Reparo", localePath)) {
    // Handle initialization error
  }

}
  void renderUI() {
    auto& loc = Localization::getInstance();

    // Language menu
    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu(_("Language"))) {
        if (ImGui::MenuItem("English", nullptr, loc.getCurrentLocale() == "en")) {
          loc.switchLanguage("en");
        }
        if (ImGui::MenuItem("Pl", nullptr, loc.getCurrentLocale() == "pl")) {
          loc.switchLanguage("pl");
        }
        if (ImGui::MenuItem("Español", nullptr, loc.getCurrentLocale() == "es")) {
          loc.switchLanguage("es");
        }
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    // Example window using gettext translations
    ImGui::Begin(_("Settings"));

    // Example of context-based translation
    ImGui::Text("%s", C_("user-settings", "Name:"));
    static char username[256] = "";
    ImGui::InputText("##username", username, sizeof(username));

    // Example of plural forms
    static int itemCount = 1;
    ImGui::InputInt(_("Items"), &itemCount);
    ImGui::Text(N_("You have %d item", "You have %d items", itemCount), itemCount);

    if (ImGui::Button(_("Save Changes"))) {
      // Handle save
    }

    ImGui::End();
  }
}