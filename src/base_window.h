#pragma once

enum WindowState {
  WindowState_Insert,
  WindowState_Update
};

class RoWindow {
public:
  virtual void Render() = 0;
  virtual ~RoWindow() = default;
  bool open = false;
  WindowState state = WindowState_Insert;
};