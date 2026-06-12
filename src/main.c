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
  struct EGLWindow *egl_window = (struct EGLWindow*)app->userData;

  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      egl_window_create_dp(egl_window);
      egl_window_create_ctx(egl_window);
      egl_window_create_sf(egl_window, app->window);

      if (!egl_window->gl_renderer.renderer_is_init) {
        egl_window_get_wd_size(egl_window, &egl_window->gl_renderer.renderer_vp_cw,
          &egl_window->gl_renderer.renderer_vp_ch);

        glViewport(0, 0, egl_window->gl_renderer.renderer_vp_cw,
          egl_window->gl_renderer.renderer_vp_ch);

        LOGI("GL viewport is set");

        egl_window->gl_renderer.renderer_is_init = true;
      }

      break;
    case APP_CMD_TERM_WINDOW:
      egl_window_destroy_sf(egl_window);

      break;
    case APP_CMD_DESTROY:
      egl_window_destroy_ctx(egl_window);
      egl_window_destroy_dp(egl_window);

      break;
  }
}

void android_main(struct android_app *app) { 
  struct EGLWindow egl_window = {
    .window_cfg_size = 1,

    .window_dp_attribs = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_NONE
    },

    .window_ctx_attribs = {
      EGL_CONTEXT_MAJOR_VERSION, 3,
      EGL_CONTEXT_MINOR_VERSION, 2,
      EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
      EGL_NONE
    }
  };

  struct AndroidCmdState cmd_state = {0};

  app->userData = &egl_window;
  app->onAppCmd = &cmd_callback_handler;

  while (true) {
    handle_cmd_state(&cmd_state, app, &egl_window);

    if (cmd_state.state_destroy_status) {
      break;
    }

    if (egl_window.window_status) {
      egl_window_get_wd_size(&egl_window, &egl_window.gl_renderer.renderer_vp_cw,
        &egl_window.gl_renderer.renderer_vp_ch);

      if (egl_window.gl_renderer.renderer_vp_cw != egl_window.gl_renderer.renderer_vp_lw ||
        egl_window.gl_renderer.renderer_vp_ch != egl_window.gl_renderer.renderer_vp_lh) {

        egl_window.gl_renderer.renderer_vp_lw = egl_window.gl_renderer.renderer_vp_cw;
        egl_window.gl_renderer.renderer_vp_lh = egl_window.gl_renderer.renderer_vp_ch;

        glViewport(0, 0, egl_window.gl_renderer.renderer_vp_lw,
          egl_window.gl_renderer.renderer_vp_lh);

        LOGI("OpenGL viewport resized");
      }

      glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      eglSwapBuffers(egl_window.window_display, egl_window.window_surface);
    }
  }
}
