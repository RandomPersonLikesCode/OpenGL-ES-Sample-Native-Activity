/*
  The primary EGL window struct for handling
  EGL window related functionality.

  Copyright (c) 2026 RandomPerson
  SPDX-License-Identifier: MIT
*/

#include <EGL/egl.h>

#include <stdbool.h>

struct EGLWindow {
  EGLDisplay window_display;
  EGLContext window_context;
  EGLSurface window_surface;

  EGLConfig window_config;
  EGLint window_cfg_num, window_cfg_size;
  
  EGLint window_dp_attribs[11];
  EGLint window_ctx_attribs[7];
  
  EGLBoolean window_fnc_res;

  /* Is window minimized/unfocused */
  bool window_status;

  struct GLRenderer gl_renderer;
};

void egl_window_get_wd_size(struct EGLWindow *window, EGLint *width,
  EGLint *height) {

  if (window->window_display != EGL_NO_DISPLAY &&
    window->window_surface != EGL_NO_SURFACE) {

    window->window_fnc_res = eglQuerySurface(window->window_display,
      window->window_surface, EGL_WIDTH, width);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_fnc_res = eglQuerySurface(window->window_display,
      window->window_surface, EGL_HEIGHT, height);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);
  }
}

void egl_window_create_dp(struct EGLWindow *window) {
  if (window->window_display == EGL_NO_DISPLAY) {
    window->window_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    window->window_fnc_res = eglInitialize(window->window_display, NULL,
      NULL);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_fnc_res = eglChooseConfig(window->window_display,
      window->window_dp_attribs, &window->window_config,
      window->window_cfg_size, &window->window_cfg_num);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    LOGI("EGL display created");
  }
}

void egl_window_create_ctx(struct EGLWindow *window) {
  if (window->window_context == EGL_NO_CONTEXT) {
    window->window_context = eglCreateContext(window->window_display,
      window->window_config, EGL_NO_CONTEXT, window->window_ctx_attribs);

    EGL_FNC_CALL_CHECK_NORETURN(window->window_context, EGL_NO_CONTEXT);

    LOGI("EGL context created");
  }
}

void egl_window_destroy_ctx(struct EGLWindow *window) {
  window->window_status = false;

  if (window->window_context != EGL_NO_CONTEXT) {
    /*
      TODO: Somehow that eglDestroyContext is not properly
      called because of unknown reason, so stuff below
      such as eglTerminate is possibly not called, but
      it's ok for now.
    */

    window->window_fnc_res = eglDestroyContext(window->window_display,
      window->window_context);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_context = EGL_NO_CONTEXT;

    LOGI("EGL context destroyed");
  }
}

void egl_window_destroy_dp(struct EGLWindow *window) {
  if (window->window_display != EGL_NO_DISPLAY) {
    window->window_fnc_res = eglTerminate(window->window_display);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_display = EGL_NO_DISPLAY;

    LOGI("EGL display destroyed");
  }
}

void egl_window_create_sf(struct EGLWindow *window,
  NativeWindowType native_window) {

  if (window->window_context != EGL_NO_CONTEXT &&
    window->window_display != EGL_NO_DISPLAY) {

    window->window_surface = eglCreateWindowSurface(window->window_display,
      window->window_config, native_window, NULL);

    EGL_FNC_CALL_CHECK_NORETURN(window->window_surface, EGL_NO_SURFACE);

    LOGI("EGL surface created");

    window->window_fnc_res = eglMakeCurrent(window->window_display,
      window->window_surface, window->window_surface,
      window->window_context);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_status = true;

    LOGI("EGL is initialized");
  }
}

void egl_window_destroy_sf(struct EGLWindow *window) {
  window->window_status = false;

  if (window->window_display != EGL_NO_DISPLAY &&
    window->window_surface != EGL_NO_SURFACE) {

    window->window_fnc_res = eglMakeCurrent(window->window_display,
      EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_fnc_res = eglDestroySurface(window->window_display,
      window->window_surface);

    EGL_FNC_CALL_CHECK(window->window_fnc_res, EGL_TRUE);

    window->window_surface = EGL_NO_SURFACE;

    LOGI("EGL surface destroyed");
  }
}
