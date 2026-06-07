/*
  The primary EGL window struct for handling
  EGL window related functionality.

  Copyright (c) 2026 RandomPerson
  SPDX-License-Identifier: MIT
*/

#include <EGL/egl.h>

#include <stdbool.h>

struct EGLWindowRenderer {
  EGLDisplay renderer_display;
  EGLContext renderer_context;
  EGLSurface renderer_surface;

  EGLConfig renderer_cfg;
  EGLint renderer_cfg_num;
  EGLint renderer_cfg_size;
  
  EGLint renderer_dp_attribs[11];
  EGLint renderer_ctx_attribs[7];

  EGLint renderer_gl_vp_crw, renderer_gl_vp_crh,
    renderer_gl_vp_lrw, renderer_gl_vp_lrh;

  bool renderer_status;
};

void egl_renderer_get_wd_size(struct EGLWindowRenderer *window, EGLint *width,
  EGLint *height) {

  EGLBoolean egl_fnc_res = EGL_FALSE;

  if (window->renderer_display != EGL_NO_DISPLAY &&
    window->renderer_surface != EGL_NO_SURFACE) {

    egl_fnc_res = eglQuerySurface(window->renderer_display,
      window->renderer_surface, EGL_WIDTH, width);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    egl_fnc_res = eglQuerySurface(window->renderer_display,
      window->renderer_surface, EGL_HEIGHT, height);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);
  }
}

void egl_renderer_create_wd(struct EGLWindowRenderer *window,
  NativeWindowType native_window) {

  EGLBoolean egl_fnc_res = EGL_FALSE;

  if (window->renderer_display == EGL_NO_DISPLAY) {
    window->renderer_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    egl_fnc_res = eglInitialize(window->renderer_display, NULL, NULL);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    egl_fnc_res = eglChooseConfig(window->renderer_display,
      window->renderer_dp_attribs, &window->renderer_cfg,
      window->renderer_cfg_size, &window->renderer_cfg_num);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    LOGI("EGL display created");
  }

  if (window->renderer_context == EGL_NO_CONTEXT) {
    window->renderer_context = eglCreateContext(window->renderer_display,
      window->renderer_cfg, EGL_NO_CONTEXT, window->renderer_ctx_attribs);

    EGL_FNC_CALL_CHECK_NORETURN(window->renderer_context, EGL_NO_CONTEXT);

    LOGI("EGL context created");
  }

  window->renderer_surface = eglCreateWindowSurface(window->renderer_display,
    window->renderer_cfg, native_window, NULL);

  EGL_FNC_CALL_CHECK_NORETURN(window->renderer_surface, EGL_NO_SURFACE);

  LOGI("EGL surface created");

  egl_fnc_res = eglMakeCurrent(window->renderer_display, window->renderer_surface,
    window->renderer_surface, window->renderer_context);

  EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

  window->renderer_status = true;

  LOGI("EGL is initialized");
}

void egl_renderer_wd_destroy_sf(struct EGLWindowRenderer *window) {
  EGLBoolean egl_fnc_res = EGL_FALSE;
  window->renderer_status = false;

  if (window->renderer_display != EGL_NO_DISPLAY &&
    window->renderer_surface != EGL_NO_SURFACE) {

    egl_fnc_res = eglMakeCurrent(window->renderer_display, EGL_NO_SURFACE,
      EGL_NO_SURFACE, EGL_NO_CONTEXT);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    egl_fnc_res = eglDestroySurface(window->renderer_display,
      window->renderer_surface);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    window->renderer_surface = EGL_NO_SURFACE;

    LOGI("EGL surface destroyed");
  }
}

void egl_renderer_wd_destroy_all(struct EGLWindowRenderer *window) {
  EGLBoolean egl_fnc_res = EGL_FALSE;
  window->renderer_status = false;

  if (window->renderer_display != EGL_NO_DISPLAY &&
    window->renderer_context != EGL_NO_CONTEXT) {

    /*
      TODO: Somehow that eglDestroyContext is not properly
      called because of unknown reason, so stuff below
      such as eglTerminate is possibly not called, but
      it's ok for now.
    */

    egl_fnc_res = eglDestroyContext(window->renderer_display,
      window->renderer_context);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    egl_fnc_res = eglTerminate(window->renderer_display);

    EGL_FNC_CALL_CHECK(egl_fnc_res, EGL_TRUE);

    window->renderer_context = EGL_NO_CONTEXT;
    window->renderer_display = EGL_NO_DISPLAY;

    LOGI("EGL display/context destroyed");
  }
}
