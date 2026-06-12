#include <android_native_app_glue.h>
#include <stdbool.h>

struct AndroidCmdState {
  int state_id;
  int state_events;
  bool state_destroy_status;

  struct android_poll_source *state_poll_src;
};

void handle_cmd_state(struct AndroidCmdState *cmd_state, struct android_app *app,
  struct EGLWindow *window) {

  while ((cmd_state->state_id = ALooper_pollOnce(window->window_status ? 0 : -1, NULL,
    &cmd_state->state_events, (void**)&cmd_state->state_poll_src)) >= 0) {
    LOGI("Command processing");

    if (cmd_state->state_poll_src != NULL) {
      cmd_state->state_poll_src->process(app, cmd_state->state_poll_src);

      LOGI("Command processed");
    }
  }

  if (app->destroyRequested != 0) {
    /*
      TODO: here is somehow is not properly executed
      just like in src/egl_window.c:101
    */

    LOGI("App exiting");

    cmd_state->state_destroy_status = true;
  }
}
