#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <GL/glew.h>

typedef struct IndexBuffer
{
    GLuint renderer_id;
    unsigned int count;
} IndexBuffer;

IndexBuffer ib_create(const GLuint* indices, GLuint indices_count);

void ib_bind(IndexBuffer* self);
void ib_unbind(void);

void ib_delete(IndexBuffer* self);

typedef struct ibns_t {
  IndexBuffer (*create)(const GLuint* indices, GLuint indices_count);
  void (*bind)(IndexBuffer* self);
  void (*unbind)(void);
  void (*delete)(IndexBuffer* self);
} ibns_t;

extern ibns_t IndexBufferNS;

#endif
