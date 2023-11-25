#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct Shader {
    GLuint id;
} Shader;

typedef struct ShaderNS_t {
  Shader (*new)(const char *vertexPath, const char *fragmentPath);
  void (*use)(Shader shader);
  void (*setInt)(Shader shader, const char *name, GLint value);
  void (*setFloat)(Shader shader, const char *name, GLfloat value);
  void (*setMat4)(Shader shader, const char *name, GLfloat *value);
  void (*setVec3)(Shader shader, const char *name, GLfloat *value);
  void (*delete)(Shader shader);
} ShaderNS_t;

extern ShaderNS_t ShaderNS;


#endif
