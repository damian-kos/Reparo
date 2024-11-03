#include "reparo_core.h"
#include "imgui.h"
#include "database.h"

void ReparoCore::Render() {

}

#ifdef DEBUG

void ReparoCore::RenderDebug() {
  ImGui::Begin("Debug");
    CreateDatabaseBtn();
  ImGui::End();
}

void ReparoCore::CreateDatabaseBtn() {
  if (ImGui::Button("Create Database")) {
    Database::CreateDatabase();
  }
}

#endif // DEBUG
