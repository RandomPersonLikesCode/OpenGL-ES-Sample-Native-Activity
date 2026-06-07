/*
  Macros for logging.

  Copyright (c) 2026 RandomPerson
  SPDX-License-Identifier: MIT
*/

#include <android/log.h>

#include <EGL/egl.h>

#define LOG_TAG "OGLESSample"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define EGL_FNC_CALL_CHECK(egl_fnc_res, egl_expected_res) do { \
    if ((egl_fnc_res) != (egl_expected_res)) {                 \
      EGLint egl_err_code = eglGetError();                     \
                                                               \
      LOGE("EGL Error: 0x%X", egl_err_code);                   \
    }                                                          \
  } while (0)

#define EGL_FNC_CALL_CHECK_NORETURN(egl_fnc_res, egl_noreturn) do { \
    if ((egl_fnc_res) == (egl_noreturn)) {                          \
      EGLint egl_err_code = eglGetError();                          \
                                                                    \
      LOGE("EGL Error: 0x%X", egl_err_code);                        \
    }                                                               \
  } while (0)
