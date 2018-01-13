#ifndef REVERSE_BUFFER_H
#define REVERSE_BUFFER_H

#include "reverse.h"

#ifndef BUFFER_CHUNK_SIZE
#define BUFFER_CHUNK_SIZE 256
#endif

#define ALIGN(V, B) (V) >= 0 ? (((V) + (B) - 1) / (B)) * (B) : ((V) / (B)) * (B)

typedef struct _buffer_t
{
    char *data;
    size_t allocated, used;
} buffer_t;

buffer_t *buffer_create(void);
unsigned char *buffer_to_bytes(const buffer_t *buf);
int buffer_append(buffer_t *buf, const void *data, size_t size);
int buffer_append_str(buffer_t *buf, const char *str);
int buffer_reset(buffer_t *buf);
void buffer_destroy(buffer_t **bufferPtr);

#endif
