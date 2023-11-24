#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "renderer.h"
void gl_clear_error(void) {
  while (glGetError() != GL_NO_ERROR);
}

void gl_check_error(void) {
  GLenum error;
  while ((error = glGetError())) {
    fprintf(stderr, "[OpenGL Error] (0x%x)\n", error);
  }
}


