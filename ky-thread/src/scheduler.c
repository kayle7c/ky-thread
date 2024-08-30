#ifndef ___SCHEDULER_H__
#define ___SCHEDULER_H__

#include "thread.h"

/* 已知一个结构体里面的成员的地址，反推出该结构体的首地址 */
#define rt_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
			
#define rt_list_entry(node, type, member) \
    rt_container_of(node, type, member)

/*线程就绪列表*/
ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

/*指向当前线程的线程控制块*/
struct ky_thread *ky_current_thread;


void ky_system_scheduler_init(void)
{
		register ky_base_t offset;
		
		for(offset=0;offset<KY_THREAD_PRIORITY_MAX;offset++)
		{
				ky_list_init(&ky_thread_priority_table[offset]);
		}
		
		ky_current_thread=KY_NULL;	
}

void ky_system_scheduler_start(void)
{
		register struct ky_thread *to_thread;
	
		to_thread=rt_list_entry(ky_thread_priority_table[0].next,
														struct ky_thread,
														tlist);
														
		ky_current_thread=to_thread;	

		rt_hw_context_switch_to((ky_uint32_t)&to_thread->sp);													
}

void ky_scheduler(void)
{
		struct ky_thread *to_thread;
		struct ky_thread *from_thread;
	
		if(ky_current_thread==rt_list_entry(ky_thread_priority_table[0].next,
														struct ky_thread,
														tlist))
		{
				from_thread=ky_current_thread;
				to_thread=rt_list_entry(ky_thread_priority_table[1].next,
														struct ky_thread,
														tlist);
														
				ky_current_thread=to_thread;			
		}
		else
		{
				from_thread=ky_current_thread;
				to_thread=rt_list_entry(ky_thread_priority_table[0].next,
														struct ky_thread,
														tlist);
														
				ky_current_thread=to_thread;	
		}
		
		rt_hw_context_switch((ky_uint32_t)&from_thread->sp,(ky_uint32_t)&to_thread->sp);
}

#endif
	