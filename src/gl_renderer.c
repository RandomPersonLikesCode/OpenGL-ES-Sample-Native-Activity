#include <GLES3/gl32.h>
#include <stdbool.h>

struct GLRenderer {
  /*
     vp viewport
     c current
     l last
     w/h width/height
  */
  GLsizei renderer_vp_cw, renderer_vp_ch, renderer_vp_lw,
    renderer_vp_lh;

  bool renderer_is_init;
};
