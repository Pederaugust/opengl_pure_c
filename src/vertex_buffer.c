#include <GL/glew.h>

#include "vertex_buffer.h"

void vb_bind(VertexBuffer* self) {
  glBindBuffer(GL_ARRAY_BUFFER, self->renderer_id);
}

void vb_unbind(void) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vb_delete(VertexBuffer* self) {
  glDeleteBuffers(1, &self->renderer_id);
}

VertexBuffer vb_create(const void* vertices, unsigned int vertices_size) {
  VertexBuffer vertex_buffer;
  glGenBuffers(1, &vertex_buffer.renderer_id);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer.renderer_id);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
  return vertex_buffer;
}

vns_t VertexBufferNS = {
  .bind = vb_bind,
  .unbind = vb_unbind,
  .delete = vb_delete,
  .create = vb_create
};

