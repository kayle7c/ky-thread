#include "def.h"
#include "list.h"
#include "hw_settings.h"

ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size)

{
		ky_list_init(&(thread->tlist));
		
		thread->entry=(void *)entry;
		thread->parameter=parameter;
		
		thread->stack_addr=stack_start;
		thread->stack_size=stack_size;
		
		thread->sp=ky_hw_stack_init(thread->entry,
														 thread->parameter,
														 thread->stack_addr+thread->stack_size-4);
	
		return KY_EOK;
}