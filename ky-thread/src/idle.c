#include "thread.h"

#define IDLE_THREAD_STACK_SIZE      512

ALIGN(KY_ALIGN_SIZE)
static ky_uint8_t ky_thread_stack[IDLE_THREAD_STACK_SIZE];

struct ky_thread idle;

extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

ky_ubase_t ky_idletask_cnt=0;

void ky_thread_idle_entry(void *parameter)
{
		parameter=parameter;
		while(1)
		{
				ky_idletask_cnt++;
		}
}

void ky_thread_idle_init(void)
{
		ky_thread_init(&idle,
									 "idle",
									 ky_thread_idle_entry,
									 KY_NULL,
									 &ky_thread_stack[0],
									 sizeof(ky_thread_stack),
									 KY_THREAD_PRIORITY_MAX-1,
									 2);
		
		ky_thread_startup(&idle);
}