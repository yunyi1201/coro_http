#pragma once

#include <stddef.h>
#include <stdint.h>

struct buffer {
  uint8_t *start, *end;
  uint8_t *pos, *last;
};

static inline void bind_buffer(struct buffer *b, void *buff, size_t len) {
  b->start = (uint8_t *)buff;
  b->end = b->start + len;
  b->pos = b->last = b->start;
}
