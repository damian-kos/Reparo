#pragma once

class RoWindow {
public:
  virtual void Render() = 0;
  virtual ~RoWindow() = default;
  bool open = false;
};