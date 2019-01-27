#include <memory>

#include <GLES/gl.h>

#include <android_native_app_glue.h>

#include "Context.hpp"

namespace Core {

void Context::initialize(android_app *pApp) {
  if(mInstialized) {
    return;
  }

  /*
   * Here specify the attributes of the desired configuration.
   * Below, we select an EGLConfig with at least 8 bits per color
   * component compatible with on-screen windows
   */
  // clang-format off
  const EGLint attribs[] = {
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_NONE
  };
  // clang-format on

  EGLint w, h, format;
  EGLint numConfigs;
  EGLConfig config;
  EGLSurface surface;
  EGLContext context;

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  eglInitialize(display, 0, 0);

  /*
   * Here, the application chooses the configuration it desires.
   * find the best match if possible, otherwise use the very first one
   */
  eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);
  std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
  eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

  auto i = 0;
  for(; i < numConfigs; i++) {
    auto &cfg = supportedConfigs[i];
    EGLint r, g, b, d;
    if(eglGetConfigAttrib(display, cfg, EGL_RED_SIZE, &r) &&
       eglGetConfigAttrib(display, cfg, EGL_GREEN_SIZE, &g) &&
       eglGetConfigAttrib(display, cfg, EGL_BLUE_SIZE, &b) &&
       eglGetConfigAttrib(display, cfg, EGL_DEPTH_SIZE, &d) && r == 8 &&
       g == 8 && b == 8 && d == 0) {

      config = supportedConfigs[i];
      break;
    }
  }

  if(i == numConfigs) {
    config = supportedConfigs[0];
  }

  /*
   * EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
   * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
   * As soon as we picked a EGLConfig, we can safely reconfigure the
   * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
   */
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
  surface = eglCreateWindowSurface(display, config, pApp->window, nullptr);
  context = eglCreateContext(display, config, nullptr, nullptr);

  if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
    //LOGW("Unable to eglMakeCurrent");
    return;
  }

  eglQuerySurface(display, surface, EGL_WIDTH, &w);
  eglQuerySurface(display, surface, EGL_HEIGHT, &h);

  mDisplay = display;
  mContext = context;
  mSurface = surface;
  mWidth   = w;
  mHeight  = h;

  // Check openGL on the system
  //auto opengl_info = {GL_VENDOR, GL_RENDERER, GL_VERSION, GL_EXTENSIONS};
  //for(auto name : opengl_info) {
  //  auto info = glGetString(name);
  //  (void)info;
  //  //LOGI("OpenGL Info: %s", info);
  //}

  mInstialized = true;
}

void Context::clean() {
  if(!mInstialized) {
    return;
  }

  if(mDisplay != EGL_NO_DISPLAY) {
    eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if(mContext != EGL_NO_CONTEXT) {
      eglDestroyContext(mDisplay, mContext);
    }
    if(mSurface != EGL_NO_SURFACE) {
      eglDestroySurface(mDisplay, mSurface);
    }
    eglTerminate(mDisplay);
  }

  mDisplay = EGL_NO_DISPLAY;
  mContext = EGL_NO_CONTEXT;
  mSurface = EGL_NO_SURFACE;

  mInstialized = false;
}

void Context::swapBuffer() {
  if(mInstialized) {
    eglSwapBuffers(mDisplay, mSurface);
  }
}

}  // namespace Core
