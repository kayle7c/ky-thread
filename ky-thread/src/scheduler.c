#include "thread.h"

extern struct ky_thread idle;
extern struct ky_thread ky_test1_thread;
extern struct ky_thread ky_test2_thread;

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
	
		to_thread=ky_list_entry(ky_thread_priority_table[0].next,
														struct ky_thread,
														tlist);
														
		ky_current_thread=to_thread;	

		rt_hw_context_switch_to((ky_uint32_t)&to_thread->sp);													
}

void ky_scheduler(void)
{
		struct ky_thread *to_thread;
		struct ky_thread *from_thread;
#if 0	
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
#else
//当前线程是空闲线程
		if(ky_current_thread==&idle)
		{
				if(ky_test1_thread.remaining_tick==0)
				{
						from_thread=ky_current_thread;
						to_thread=&ky_test1_thread;
						ky_current_thread=to_thread;
				}
				else if(ky_test2_thread.remaining_tick==0)
				{
						from_thread=ky_current_thread;
						to_thread=&ky_test2_thread;
						ky_current_thread=to_thread;	
				}
				else return;
		}
		else  //当前线程不是空闲线程
		{
				if(ky_current_thread==&ky_test1_thread)
				{
						if(ky_test2_thread.remaining_tick!=0)
						{
								from_thread=ky_current_thread;
								to_thread=&ky_test2_thread;
								ky_current_thread=to_thread;
						}
						else if(ky_current_thread->remaining_tick!=0)
						{
								from_thread=ky_current_thread;
								to_thread=&idle;
								ky_current_thread=to_thread;
						}
						else //当前线程没有进入延时
						{
								return;           
						}
				}
				else if(ky_current_thread==&ky_test2_thread)
				{
						if(ky_test1_thread.remaining_tick!=0)
						{
								from_thread=ky_current_thread;
								to_thread=&ky_test1_thread;
								ky_current_thread=to_thread;
						}
						else if(ky_current_thread->remaining_tick!=0)
						{
								from_thread=ky_current_thread;
								to_thread=&idle;
								ky_current_thread=to_thread;
						}
						else //当前线程没有进入延时
						{
								return;           
						}
				}
	
		}
#endif	
		rt_hw_context_switch((ky_uint32_t)&from_thread->sp,(ky_uint32_t)&to_thread->sp);
}