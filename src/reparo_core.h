#pragma once

class ReparoCore {
  void Render();

#ifdef DEBUG
public:
  static void RenderDebug();

private:  
  static void CreateDatabaseBtn();
#endif // DEBUG

};