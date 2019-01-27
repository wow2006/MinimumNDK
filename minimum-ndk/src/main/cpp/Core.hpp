#pragma once
#include <cstdint>

#include "Context.hpp"

struct android_app;
struct AInputEvent;
struct android_poll_source;

namespace Core {
class Engine {
public:
  Engine();

  ~Engine();

  void initializeDisplay();

  void drawFrame();

  void termDisplay();

  static void HandleCMD(android_app *pApp, int32_t cmd);

  static int32_t HandleInput(android_app *pApp, AInputEvent *pEvent);

  android_app *m_pApp = nullptr;

protected:
  Context mContext;
};
}  // namespace Core
