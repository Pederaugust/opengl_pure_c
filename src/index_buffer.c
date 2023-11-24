#include <GL/glew.h>
#include "index_buffer.h"

void ib_bind(IndexBuffer* self) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->renderer_id);
}

void ib_unbind(void) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ib_delete(IndexBuffer* self) {
  glDeleteBuffers(1, &self->renderer_id);
}

IndexBuffer ib_create(const GLuint* indices, GLuint indices_count) {
  IndexBuffer index_buffer;
  index_buffer.count = indices_count;
  glGenBuffers(1, &index_buffer.renderer_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.renderer_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_count * sizeof(GLuint), indices, GL_STATIC_DRAW);
  return index_buffer;
}

ibns_t IndexBufferNS = {
  .bind = ib_bind,
  .unbind = ib_unbind,
  .delete = ib_delete,
  .create = ib_create
};



