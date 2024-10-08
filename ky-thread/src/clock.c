#include "thread.h"

ky_tick_t ky_tick = 0;    //ϵͳʱ��������
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];
extern ky_uint32_t ky_thread_ready_priority_group;

#if 0
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
#else
void ky_tick_increase(void)
{
		struct ky_thread *thread;
		
		ky_tick++;
	
		thread = ky_thread_self();
	
		thread->remaining_tick--; //ʱ��Ƭ�ݼ�
	
		//���ʱ��Ƭ���꣬����ʱ��Ƭ����ó�������
		if(thread->remaining_tick == 0)
		{
				thread->remaining_tick = thread->init_tick;
				
				ky_thread_yield();
		}
	
		ky_timer_check();
}

#endif
ky_tick_t ky_tick_get(void)
{
		return ky_tick;
}