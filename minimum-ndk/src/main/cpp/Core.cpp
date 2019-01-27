#include "Core.hpp"

#include <GLES/gl.h>

#include <android_native_app_glue.h>

namespace Core {
Engine::Engine() = default;

Engine::~Engine() = default;

void Engine::initializeDisplay() { mContext.initialize(m_pApp); }

void Engine::drawFrame() {
  // Initialize GL state.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
  glDisable(GL_DEPTH_TEST);

  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  mContext.swapBuffer();
}

void Engine::termDisplay() { mContext.clean(); }

void Engine::HandleCMD(android_app *pApp, int32_t cmd) {
  auto pEngine = reinterpret_cast<Engine *>(pApp->userData);

  switch(cmd) {
  case APP_CMD_SAVE_STATE: break;
  case APP_CMD_INIT_WINDOW:
    if(pEngine->m_pApp->window != nullptr) {
      pEngine->initializeDisplay();
      pEngine->drawFrame();
    }
    break;
  case APP_CMD_TERM_WINDOW: {
    pEngine->termDisplay();
  } break;
  case APP_CMD_GAINED_FOCUS: break;
  case APP_CMD_LOST_FOCUS: break;
  default: break;
  }
}

int32_t Engine::HandleInput(android_app *pApp, AInputEvent *pEvent) {
  return 0;
}

}  // namespace Core
