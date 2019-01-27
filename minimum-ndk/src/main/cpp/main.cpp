#include <android_native_app_glue.h>

#include "Core.hpp"

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *pState) {
  Core::Engine engine;

  pState->userData     = &engine;
  pState->onAppCmd     = Core::Engine::HandleCMD;
  pState->onInputEvent = Core::Engine::HandleInput;

  engine.m_pApp = pState;

  // loop waiting for stuff to do.
  while(true) {
    // Read all pending events.
    int ident;
    int events;
    android_poll_source *pSource = nullptr;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while((ident = ALooper_pollAll(
             -1, nullptr, &events, reinterpret_cast<void **>(&pSource))) >= 0) {
      // Process this event.
      if(pSource != nullptr) {
        pSource->process(pState, pSource);
      }

      // Check if we are exiting.
      if(pState->destroyRequested != 0) {
        engine.termDisplay();
        return;
      }

      engine.drawFrame();
    }
  }
}
