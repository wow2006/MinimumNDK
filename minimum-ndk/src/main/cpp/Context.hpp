#pragma once
#include <EGL/egl.h>

struct android_app;

namespace Core {
struct Context {
  void initialize(android_app *pApp);

  void clean();

  void swapBuffer();

protected:
  bool mInstialized = false;

  EGLDisplay mDisplay;
  EGLContext mContext;
  EGLSurface mSurface;
  unsigned int mWidth;
  unsigned int mHeight;
};
}  // namespace Core
