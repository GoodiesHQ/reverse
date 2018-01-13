#include "buffer.h"

/*
 * creates a buffer object. Returns NULL on failure;
 */
buffer_t *buffer_create(void)
{
    buffer_t *buf;
    if((buf = calloc(1, sizeof(buffer_t))) == NULL)
    {
        debugf("%s\n", "calloc error");
        return NULL;
    }
    return buf;
}

/*
 * converts a buffer to a char array and returns a copy. Does not free/alter the buffer.
 */
unsigned char *buffer_to_bytes(const buffer_t *buf)
{
    unsigned char *ret;
    if((ret = malloc(buf->used)) == NULL)
    {
        debugf("%s\n", "malloc error");
        return NULL;
    }
    memmove(ret, buf->data, buf->used);
    return ret;
}

/*
 * appends data to a buffer with a given size. Buffer dynamically grows as needed.
 */
int buffer_append(buffer_t *buf, const void *data, size_t size)
{
    char *tmp;
    const size_t orig_used = buf->used, orig_allocated = buf->allocated;

    if(size == 0)
    {
        return 0;
    }

    buf->used += size;

    if(buf->allocated <= buf->used)
    {
        buf->allocated = ALIGN(buf->used, BUFFER_CHUNK_SIZE);
        if((tmp = realloc(buf->data, buf->allocated)) == NULL)
        {
            buf->allocated = orig_allocated;
            buf->used = orig_used;
            debugf("%s\n", "realloc error.");
            return 1;
        }
        buf->data = tmp;
    }
    memcpy(buf->data + orig_used, data, size);
    return 0;
}

/*
 * appends string data to a buffer.
 */
int buffer_append_str(buffer_t *buf, const char *str)
{
    return buffer_append(buf, (const void*)str, strlen(str));
}

/*
 * Resets a buffer so that it contains no allocated data.
 */
int buffer_reset(buffer_t *buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->allocated=0;
    buf->used = 0;
    return 0;
}

/*
 * deallocaes a buffer.
 */
void buffer_destroy(buffer_t **bufferPtr)
{
    buffer_t *buffer = *bufferPtr;
    if(buffer)
    {
        free(buffer->data);
        free(buffer);
        *bufferPtr = NULL;
    }
}
