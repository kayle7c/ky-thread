#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include "thread.h"

enum ky_ringbuffer_state
{
    KY_RINGBUFFER_EMPTY,
    KY_RINGBUFFER_FULL,
    KY_RINGBUFFER_HALFFULL,
};

struct ky_ringbuffer
{
		ky_uint8_t *ringbuffer;
	
		ky_uint16_t read_mirror:1;
		ky_uint16_t read_index:15;
	
		ky_uint16_t write_mirror:1;
		ky_uint16_t write_index:15;
	
		ky_int16_t buffer_size;
};

void ky_ringbuffer_init(struct ky_ringbuffer* rb,ky_uint8_t* pool,ky_int16_t size);
ky_size_t ky_ringbuffer_getchar(struct ky_ringbuffer* rb,ky_uint8_t* ch);
ky_size_t ky_ringbuffer_putchar(struct ky_ringbuffer* rb,ky_uint8_t ch);
ky_size_t ky_ringbuffer_isempty(struct ky_ringbuffer* rb);


#endif