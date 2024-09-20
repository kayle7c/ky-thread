#include "thread.h"
#include "ringbuffer.h"

void ky_ringbuffer_init(struct ky_ringbuffer* rb,ky_uint8_t* pool,ky_int16_t size)
{
		rb->read_mirror=0;
		rb->write_mirror=0;
		rb->read_index=0;
		rb->write_index=0;
	
		rb->ringbuffer=pool;
		rb->buffer_size = KY_ALIGN_DOWN(size, 4);
}

ky_size_t ky_ringbuffer_getchar(struct ky_ringbuffer* rb,ky_uint8_t* ch)
{
		*ch=rb->ringbuffer[rb->read_index];
	
		if(rb->read_index==rb->buffer_size-1)
		{
				rb->read_index=0;
		}
		else
		{
				rb->read_index++;
		}
		return 1;
}

ky_size_t ky_ringbuffer_putchar(struct ky_ringbuffer* rb,ky_uint8_t ch)
{
		rb->ringbuffer[rb->write_index]=ch;
	
		if(rb->write_index==rb->buffer_size-1)
		{
				rb->write_index=0;
		}
		else
		{
				rb->write_index++;
		}
		return 1;
}

ky_size_t ky_ringbuffer_isempty(struct ky_ringbuffer* rb)
{
//		printf("%d",rb->write_index==rb->read_index);
		return rb->write_index==rb->read_index;
}