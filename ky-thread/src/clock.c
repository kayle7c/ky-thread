#include "thread.h"

static ky_tick_t ky_tick = 0;    //系统时基计数器
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

void ky_tick_increase(void)
{
		struct ky_thread *thread;
	
		for(ky_ubase_t i=0;i<KY_THREAD_PRIORITY_MAX;i++)//遍历就绪序列
		{
				thread=ky_list_entry(ky_thread_priority_table[i].next,struct ky_thread,tlist);
				
				if(thread->remaining_tick>0)//如果正在延时就递减
				{
						thread->remaining_tick--;
				}
		}
		ky_scheduler();
}