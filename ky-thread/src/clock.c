#include "thread.h"

static ky_tick_t ky_tick = 0;    //ϵͳʱ��������
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

void ky_tick_increase(void)
{
		struct ky_thread *thread;
	
		for(ky_ubase_t i=0;i<KY_THREAD_PRIORITY_MAX;i++)//������������
		{
				thread=ky_list_entry(ky_thread_priority_table[i].next,struct ky_thread,tlist);
				
				if(thread->remaining_tick>0)//���������ʱ�͵ݼ�
				{
						thread->remaining_tick--;
				}
		}
		ky_scheduler();
}