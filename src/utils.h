#ifndef UTILS_H
#define UTILS_H

GLuint load_shaders(const char *, const char *);

typedef struct utils_t {
  GLuint (*load_shaders)(const char *, const char *);
} utils_t;

extern utils_t Utils;

#endif
