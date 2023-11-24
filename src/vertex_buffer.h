#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H


typedef struct VertexBuffer
{
    GLuint renderer_id;
} VertexBuffer;

VertexBuffer vb_create(const void* vertices, unsigned int vertices_size);

void vb_bind(VertexBuffer* self);
void vb_unbind(void);

void vb_delete(VertexBuffer* self);


typedef struct vns_t {
  VertexBuffer (*create)(const void* vertices, unsigned int vertices_size);
  void (*bind)(VertexBuffer* self);
  void (*unbind)(void);
  void (*delete)(VertexBuffer* self);
} vns_t;

extern vns_t VertexBufferNS;

#endif
