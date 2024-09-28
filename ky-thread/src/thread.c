#include "def.h"
#include "list.h"
#include "hw_settings.h"
#include "thread.h"

extern struct ky_thread *ky_current_thread;
extern ky_uint32_t ky_thread_ready_priority_group;
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

ky_thread_t thread_array[10];
ky_size_t thread_cnt=0;

ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size,
												ky_uint8_t priority,
												ky_uint32_t tick)

{
//		ky_object_init((ky_object_t)thread, KY_Object_Class_Thread, name);
		thread_array[thread_cnt]=thread;
		thread_cnt++;
	
		ky_list_init(&(thread->tlist));
		
		thread->name=name;
		thread->entry=(void *)entry;
		thread->parameter=parameter;
		
		thread->stack_addr=stack_start;
		thread->stack_size=stack_size;
		
	  //初始化线程栈，并返回sp指针
		thread->sp=(void *)ky_hw_stack_init(thread->entry,
														 thread->parameter,
										(void *)((char *)thread->stack_addr+thread->stack_size-4));
		
		thread->init_priority=priority;
		thread->current_priority=priority;
		thread->number_mask=0;
	
		thread->error=KY_EOK;
	  thread->stat=KY_THREAD_INIT;
		
		//初始化时间片
		thread->init_tick=tick;
		thread->remaining_tick=tick;
		
		thread->run_time=0;
		
		//初始化定时器
		ky_timer_init(&(thread->thread_timer),
									thread->name,
									ky_thread_timeout,
									thread,
									0,
									KY_TIMER_FLAG_ONE_SHOT);
	
		return KY_EOK;
}

//挂起线程->修改状态，从就绪列表中删除，线程计时器停止计时
ky_err_t ky_thread_suspend(ky_thread_t thread)
{
		register ky_base_t temp;
	
		if((thread->stat & KY_THREAD_STAT_MASK) != KY_THREAD_READY)
		{
				return -KY_ERROR;
		}
		
		temp = rt_hw_interrupt_disable();  
		
		//改变线程为挂起态
		thread->stat=KY_THREAD_SUSPEND;
		
		//将线程从就绪列表中删除
		ky_schedule_remove_thread(thread);
	
		//停止线程计时器
		ky_timer_stop(&(thread->thread_timer));
		
		rt_hw_interrupt_enable(temp);
		
		return KY_EOK;
}

ky_err_t ky_thread_sleep(ky_tick_t tick)
{
		register ky_base_t temp;
		struct ky_thread *thread;
	
		temp = rt_hw_interrupt_disable();
	
		//获取当前线程的线程控制块
		thread=ky_current_thread;
		
		//挂起线程
		ky_thread_suspend(thread);
		
		//设置定时器的延时时间
		ky_timer_control(&(thread->thread_timer),KY_TIMER_CTRL_SET_TIME,&tick);
	
		//启动定时器
		ky_timer_start(&(thread->thread_timer));     
	
		rt_hw_interrupt_enable(temp);  
	
		//开始调度
		ky_schedule();
}

ky_err_t ky_thread_delay(ky_tick_t tick)
{
		return ky_thread_sleep(tick);
}

void ky_thread_delay_ms(ky_tick_t ms)
{
		ky_thread_delay(ms);
}

void ky_thread_delay_s(ky_tick_t s)
{
		ky_thread_delay_ms(s*1000);
}

ky_thread_t ky_thread_self(void)
{
		return ky_current_thread;
}

//恢复一个线程然后将其放到就绪列表
ky_err_t ky_thread_resume(ky_thread_t thread)
{
		register ky_base_t temp;
	
		//被恢复的线程必须在挂起态
		if((thread->stat & KY_THREAD_STAT_MASK) != KY_THREAD_SUSPEND)
		{
				return -KY_ERROR;
		}
	
		temp = rt_hw_interrupt_disable();
		
		//从挂起队列移除
		ky_list_remove(&(thread->tlist));
		
		rt_hw_interrupt_enable(temp);
		
		ky_schedule_insert_thread(thread);
	
		return KY_EOK;
}

//启动一个线程并将其放到系统的就绪列表中
ky_err_t ky_thread_startup(ky_thread_t thread)
{
		thread->current_priority=thread->init_priority;
		thread->number_mask=1L << thread->current_priority;
	
		//改变线程状态为挂起状态
		thread->stat=KY_THREAD_SUSPEND;
	
		ky_thread_resume(thread);                                 

    if (ky_thread_self() != KY_NULL)                         
    {
        /* 系统调度 */
        ky_schedule();                                        
    }

    return KY_EOK;
}

void ky_thread_timeout(void *parameter) //此处的parameter参数就是该线程的函数入口地址
{
		struct ky_thread *thread;
		thread = (struct ky_thread *)parameter;
	
		//设置错误码为超时
		thread->error=-KY_ETIMEOUT;
	
		ky_list_remove(&(thread->tlist));
	
		//将线程插入到就绪列表里
		ky_schedule_insert_thread(thread);
	
		//系统调度
		ky_schedule();
}

//如果时间片到了让出处理器
ky_err_t ky_thread_yeild(void)
{
		register ky_base_t level;
		struct ky_thread *thread;
	
		level = rt_hw_interrupt_disable();
	
		thread = ky_current_thread;
		
		//如果线程在就绪态，并且同优先级后还有线程
		if((thread->stat & KY_THREAD_STAT_MASK)==KY_THREAD_READY && thread->tlist.next != thread->tlist.prev)
		{
				//将当前线程从就绪列表中移除
				ky_list_remove(&(thread->tlist));
			
				//将该优先级的线程序列插入到该线程的前面，这样该线程就会在这个优先级的序列的最后面了
				ky_list_insert_before(&(ky_thread_priority_table[thread->current_priority]),
                            &(thread->tlist));
			
				rt_hw_interrupt_enable(level);
			
				ky_schedule();
			
				return KY_EOK;
		}
		
		rt_hw_interrupt_enable(level);
		
		return KY_EOK;
}

ky_err_t ky_thread_control(ky_thread_t thread,int cmd,void *arg)
{
		
}