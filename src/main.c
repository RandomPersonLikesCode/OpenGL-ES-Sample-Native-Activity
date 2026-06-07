/*
  Basic OpenGL ES example to clear viewport color.

  Copyright (c) 2026 RandomPerson
  SPDX-License-Identifier: MIT
*/

#include <android_native_app_glue.h>

#include <stdbool.h>
#include <stdint.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl32.h>

void cmd_callback_handler(struct android_app *app, int32_t cmd) {
  struct EGLWindowRenderer *egl_window = (struct EGLWindowRenderer*)app->userData;

  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      egl_renderer_create_wd(egl_window, app->window);

      egl_renderer_get_wd_size(egl_window, &egl_window->renderer_gl_vp_crw,
        &egl_window->renderer_gl_vp_crh);

      glViewport(0, 0, egl_window->renderer_gl_vp_crw,
        egl_window->renderer_gl_vp_crh);

      break;
    case APP_CMD_TERM_WINDOW:
      egl_renderer_wd_destroy_sf(egl_window);

      break;

    case APP_CMD_DESTROY:
      egl_renderer_wd_destroy_all(egl_window);

      break;
  }
}

void android_main(struct android_app *app) { 
  struct EGLWindowRenderer egl_window = {
    .renderer_cfg_size = 1,

    .renderer_dp_attribs = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_NONE
    },

    .renderer_ctx_attribs = {
      EGL_CONTEXT_MAJOR_VERSION, 3,
      EGL_CONTEXT_MINOR_VERSION, 2,
      EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
      EGL_NONE
    }
  };

  app->userData = &egl_window;
  app->onAppCmd = &cmd_callback_handler;

  while (true) {
    int id = 0;
    int events = 0;
    struct android_poll_source *poll_src = NULL;

    while ((id = ALooper_pollOnce(egl_window.renderer_status ? 0 : -1,
      NULL, &events, (void**)&poll_src)) >= 0) {

      if (poll_src != NULL) {
        poll_src->process(app, poll_src);
      }
    }

    if (app->destroyRequested != 0) {
      break;
    }

    if (egl_window.renderer_status) {
      egl_renderer_get_wd_size(&egl_window, &egl_window.renderer_gl_vp_crw,
        &egl_window.renderer_gl_vp_crh);

      if (egl_window.renderer_gl_vp_crw != egl_window.renderer_gl_vp_lrw ||
        egl_window.renderer_gl_vp_crh != egl_window.renderer_gl_vp_lrh) {

        egl_window.renderer_gl_vp_lrw = egl_window.renderer_gl_vp_crw;
        egl_window.renderer_gl_vp_lrh = egl_window.renderer_gl_vp_crh;

        glViewport(0, 0, egl_window.renderer_gl_vp_lrw,
          egl_window.renderer_gl_vp_lrh);

        LOGI("OpenGL Viewport Resized");
      }

      glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      eglSwapBuffers(egl_window.renderer_display, egl_window.renderer_surface);
    }
  }
}
