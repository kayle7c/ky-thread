#include "thread.h"

struct ky_semaphore semaphores[KY_SEM_NUM_MAX];
ky_size_t sem_index=0;

ky_inline ky_err_t ky_ipc_list_suspend(ky_list_t        *list,
																			 struct ky_thread *thread,
																			 ky_uint8_t        flag)
{
		ky_thread_suspend(thread);
		
		switch(flag)
		{
		case KY_IPC_FLAG_FIFO:
				ky_list_insert_before(list, &(thread->tlist));
				break;
		
		case KY_IPC_FLAG_PRIO:                 //���ȼ���������ģʽ��δʵ��
				break;
		}
	
}

ky_inline ky_err_t ky_ipc_list_resume(ky_list_t *list)
{
		struct ky_thread *thread;
	
		thread = ky_list_entry(list->next,struct ky_thread,tlist);
	
		ky_thread_resume(thread);
}


ky_sem_t ky_sem_creat(const char *name,
											ky_uint16_t value,
											ky_uint8_t flag)
{
		struct ky_semaphore *sem = &semaphores[sem_index++];
		ky_list_init(&sem->suspend_thread);
	
		sem->value=value;
		sem->flag=flag;
		return sem;
}

ky_err_t ky_sem_release(ky_sem_t sem)
{
		register ky_base_t temp;
		register ky_bool_t need_schedule;
	
		need_schedule=KY_FALSE;
	
		//�����ٽ���
		temp = rt_hw_interrupt_disable();
	
		//����ź������������������̣߳�Ҳ˵����ǰ�ź����޿��õģ�
		if(!ky_list_isempty(&sem->suspend_thread))
		{
				//���ѵ�һ���߳�
				ky_ipc_list_resume(&sem->suspend_thread);
				
				need_schedule=KY_TRUE;
		}
		else									//���û���ź�������������û���߳�
		{
				sem->value++;
		}
		
		rt_hw_interrupt_enable(temp);
		//�����Ҫ����
		if(need_schedule == KY_TRUE)
		{
				ky_schedule();
		}
		
	  return KY_EOK;
}

ky_err_t ky_sem_take(ky_sem_t sem,ky_int32_t time)
{
		register ky_base_t temp;
		struct ky_thread *thread;
	
		//�����ٽ���
		temp = rt_hw_interrupt_disable();
	
		//����п��õ��ź���
		if(sem->value>0)
		{
				sem->value--;
				//�˳��ٽ���
				rt_hw_interrupt_enable(temp);
		}
		else
		{
				//���ȴ�
				if(time==0)
				{
						rt_hw_interrupt_enable(temp);
						return -KY_ETIMEOUT;
				}
				else
				{
						thread=ky_thread_self();
						thread->error = KY_EOK;
						
						ky_ipc_list_suspend(&sem->suspend_thread,
																thread,
																sem->flag);
						//�еȴ�ʱ��Ϳ����̵߳ļ�ʱ��������Ϊ�˺�wait_forever��-1�����֣�
						if(time>0)
						{
								ky_timer_control(&thread->thread_timer,
																 KY_TIMER_CTRL_SET_TIME,
																 &time);
								ky_timer_start(&thread->thread_timer);
						}
						//�˳��ٽ���
						rt_hw_interrupt_enable(temp);
						
						ky_schedule();
						
						if(thread->error != KY_EOK)
						{
								return thread->error;
						}
				}
		}	
		return KY_EOK;
}