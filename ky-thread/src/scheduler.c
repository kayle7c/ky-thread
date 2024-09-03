#include "thread.h"

extern struct ky_thread idle;
extern struct ky_thread ky_test1_thread;
extern struct ky_thread ky_test2_thread;

/*线程就绪列表*/
ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

/*线程就绪优先级组*/
ky_uint32_t ky_thread_ready_priority_group;

/* 当前优先级 */
ky_uint8_t ky_current_priority;

/*指向当前线程的线程控制块*/
struct ky_thread *ky_current_thread;


void ky_system_schedule_init(void)
{
#if 0
		register ky_base_t offset;
		
		for(offset=0;offset<KY_THREAD_PRIORITY_MAX;offset++)
		{
				ky_list_init(&ky_thread_priority_table[offset]);
		}
		
		ky_current_thread=KY_NULL;	
#endif 
		register ky_base_t offset;
		
		for(offset=0;offset<KY_THREAD_PRIORITY_MAX;offset++)
		{
				ky_list_init(&ky_thread_priority_table[offset]);
		}
		
		ky_current_priority=KY_THREAD_PRIORITY_MAX-1;
		
		ky_current_thread=KY_NULL;	
		
		ky_thread_ready_priority_group=0;
}

void ky_system_schedule_start(void)
{
#if 0
		register struct ky_thread *to_thread;
	
		to_thread=ky_list_entry(ky_thread_priority_table[0].next,
														struct ky_thread,
														tlist);
														
		ky_current_thread=to_thread;	

		rt_hw_context_switch_to((ky_uint32_t)&to_thread->sp);				
#else
		register struct ky_thread *to_thread;
		register ky_ubase_t highest_ready_priority;
	
		//获取最高优先级
		highest_ready_priority=__ky_ffs(ky_thread_ready_priority_group)-1;
	
		//获取最高优先级对应的线程
		to_thread = ky_list_entry(ky_thread_priority_table[highest_ready_priority].next,
                            struct ky_thread,
                            tlist);
		
		ky_current_thread = to_thread;					
															
		rt_hw_context_switch_to((ky_uint32_t)&to_thread->sp);
#endif
}

void ky_schedule(void)
{
#if 0	
		struct ky_thread *to_thread;
		struct ky_thread *from_thread;	
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
						if(ky_test2_thread.remaining_tick==0)
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
						if(ky_test1_thread.remaining_tick==0)
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
#else		
		ky_base_t level;
		register ky_ubase_t highest_ready_priority;
		struct ky_thread *to_thread;
		struct ky_thread *from_thread;
		
		level = rt_hw_interrupt_disable();		
		
		//获取最高优先级
		highest_ready_priority=__ky_ffs(ky_thread_ready_priority_group)-1;
	
		//获取最高优先级对应的线程
		to_thread = ky_list_entry(ky_thread_priority_table[highest_ready_priority].next,
                            struct ky_thread,
                            tlist);
		
		//如果目标线程不是当前的线程，就需要调度												
		if(to_thread != ky_current_thread)
		{
				ky_current_priority = (ky_uint8_t)highest_ready_priority;
				from_thread=ky_current_thread;
				ky_current_thread=to_thread;
			
				rt_hw_context_switch((ky_uint32_t)&from_thread->sp,
                            (ky_uint32_t)&to_thread->sp);
			
				rt_hw_interrupt_enable(level);
		}		
		else
		{
				rt_hw_interrupt_enable(level);
		}
		
#endif	
		rt_hw_context_switch((ky_uint32_t)&from_thread->sp,(ky_uint32_t)&to_thread->sp);
}

void ky_schedule_insert_thread(struct ky_thread *thread)
{	
		register ky_base_t temp;
	
		temp=rt_hw_interrupt_disable();
	
		ky_list_insert_before(&(ky_thread_priority_table[thread->current_priority]),&(thread->tlist));
	
		ky_thread_ready_priority_group&= ~thread->number_mask;
	
		rt_hw_interrupt_enable(temp);
}

void ky_schedule_remove_thread(struct ky_thread *thread)
{
		register ky_base_t temp;
	
		temp=rt_hw_interrupt_disable();
	
		ky_list_remove(&(thread->tlist));
	
		if(ky_list_isempty(&(ky_thread_priority_table[thread->current_priority])))
		{
				ky_thread_ready_priority_group &= ~thread->number_mask;
		}
		
		rt_hw_interrupt_enable(temp);
}