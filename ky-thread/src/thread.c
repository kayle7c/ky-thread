#include "def.h"
#include "list.h"
#include "hw_settings.h"

extern struct ky_thread *ky_current_thread;
extern ky_uint32_t ky_thread_ready_priority_group;

ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size,
												ky_uint8_t priority)

{
		ky_object_init((ky_object_t)thread, KY_Object_Class_Thread, name);
	
		ky_list_init(&(thread->tlist));
		
		thread->entry=(void *)entry;
		thread->parameter=parameter;
		
		thread->stack_addr=stack_start;
		thread->stack_size=stack_size;
		
		thread->sp=(void *)ky_hw_stack_init(thread->entry,
														 thread->parameter,
										(void *)((char *)thread->stack_addr+thread->stack_size-4));
		
		thread->init_priority=priority;
		thread->current_priority=priority;
		thread->number_mask=0;
	
		thread->error=KY_EOK;
	  thread->stat=KY_THREAD_INIT;
	
		return KY_EOK;
}

void ky_thread_delay(ky_tick_t tick)
{
#if 0
		struct ky_thread *thread;
	
		thread=ky_current_thread;
	
		thread->remaining_tick=tick;
	
		ky_schedule();
#else 
		register ky_base_t temp;
    struct ky_thread *thread;
	
		temp = rt_hw_interrupt_disable();
	
		thread = ky_current_thread;
    thread->remaining_tick = tick;
	
		//����ǰ�̹߳���
		thread->stat = KY_THREAD_SUSPEND;
		ky_thread_ready_priority_group &= ~thread->number_mask;
	
		rt_hw_interrupt_enable(temp);
		
		ky_schedule();
#endif
}

ky_thread_t ky_thread_self(void)
{
		return ky_current_thread;
}

//�ָ�һ���߳�Ȼ����ŵ������б�
ky_err_t ky_thread_resume(ky_thread_t thread)
{
		register ky_base_t temp;
	
		//���ָ����̱߳����ڹ���̬
		if((thread->stat & KY_THREAD_STAT_MASK) != KY_THREAD_SUSPEND)
		{
				return -KY_ERROR;
		}
	
		temp = rt_hw_interrupt_disable();
		
		//�ӹ�������Ƴ�
		ky_list_remove(&(thread->tlist));
		
		rt_hw_interrupt_enable(temp);
		
		ky_schedule_insert_thread(thread);
	
		return KY_EOK;
}

//����һ���̲߳�����ŵ�ϵͳ�ľ����б���
ky_err_t ky_thread_startup(ky_thread_t thread)
{
		thread->current_priority=thread->init_priority;
		thread->number_mask=1L << thread->current_priority;
	
		//�ı��߳�״̬Ϊ����״̬
		thread->stat=KY_THREAD_SUSPEND;
	
		ky_thread_resume(thread);                                 

    if (ky_thread_self() != KY_NULL)                         
    {
        /* ϵͳ���� */
        ky_schedule();                                        
    }

    return KY_EOK;
}