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
		
		case KY_IPC_FLAG_PRIO:                 //优先级阻塞唤醒模式还未实现
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
		
		sem->value=value;
		sem->flag=flag;
		return sem;
}

ky_err_t ky_sem_release(ky_sem_t sem)
{
		register ky_base_t temp;
		register ky_bool_t need_schedule;
	
		need_schedule=KY_FALSE;
	
		//进入临界区
		temp = rt_hw_interrupt_disable();
	
		//如果信号量的阻塞队列里有线程（也说明当前信号量无可用的）
		if(!ky_list_isempty(&sem->suspend_thread))
		{
				//唤醒第一个线程
				ky_ipc_list_resume(&sem->suspend_thread);
				
				need_schedule=KY_TRUE;
		}
		else									//如果没有信号量的阻塞队列没有线程
		{
				sem->value++;
		}
		
		rt_hw_interrupt_enable(temp);
		//如果需要调度
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
	
		//进入临界区
		temp = rt_hw_interrupt_disable();
	
		//如果有可用的信号量
		if(sem->value>0)
		{
				sem->value--;
				//退出临界区
				rt_hw_interrupt_enable(temp);
		}
		else
		{
				//不等待
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
						//有等待时间就开启线程的计时器（这里为了和wait_forever（-1）区分）
						if(time>0)
						{
								ky_timer_control(&thread->thread_timer,
																 KY_TIMER_CTRL_SET_TIME,
																 &time);
								ky_timer_start(&thread->thread_timer);
						}
						//退出临界区
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