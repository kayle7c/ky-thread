#include "thread.h"

static ky_tick_t ky_tick = 0;    //ϵͳʱ��������
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];
extern ky_uint32_t ky_thread_ready_priority_group;

void ky_tick_increase(void)
{
		struct ky_thread *thread;
	
		ky_tick++;
#if 0
		for(ky_ubase_t i=0;i<KY_THREAD_PRIORITY_MAX;i++)//������������
		{
				thread=ky_list_entry(ky_thread_priority_table[i].next,struct ky_thread,tlist);
				
				if(thread->remaining_tick>0)//���������ʱ�͵ݼ�
				{
						thread->remaining_tick--;
				}
		}
#else
	
		for(int i=0;i<KY_THREAD_PRIORITY_MAX;i++)
		{
				thread = ky_list_entry( ky_thread_priority_table[i].next,
                                struct ky_thread,
                                tlist);
				
				if(thread->remaining_tick>0)//���������ʱ�͵ݼ�
				{
						thread->remaining_tick--;
						if(thread->remaining_tick==0)
						{
							//rt_schedule_insert_thread(thread);
                ky_thread_ready_priority_group |= thread->number_mask; 
						}
				}												
		}
			
		
#endif
		ky_schedule();
}