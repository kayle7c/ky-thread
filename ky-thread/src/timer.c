#include "thread.h"

//定时器列表
static ky_list_t ky_timer_list[KY_TIMER_SKIP_LIST_LEVEL];

void ky_system_timer_init()
{
		for(int i=0;i<sizeof(ky_timer_list)/sizeof(ky_timer_list[0]);i++)
		{
				ky_list_init(ky_timer_list+i);
		}		
}

void _ky_timer_init(ky_timer_t timer,
									  void(*timeout)(void *parameter),
									  void *parameter,
									  ky_tick_t time,
									  ky_uint8_t flag)
{
		timer->parent.flag=flag;
		
		//设置为非激活态
		timer->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
		
		timer->timer_func=timeout;
		timer->parameter=parameter;
			
		timer->timeout_tick=0;
		timer->init_tick=time;
	
		for(int i=0;i<KY_TIMER_SKIP_LIST_LEVEL;i++)
		{
				ky_list_init(&(timer->row[i]));
		}
	
}

//初始化定时器
void ky_timer_init(ky_timer_t timer,
									 const char *name,
									 void(*timeout)(void *parameter),
									 void *parameter,
									 ky_tick_t time,
									 ky_uint8_t flag)
{
		ky_object_init((ky_object_t)timer, KY_Object_Class_Timer, name);
		
		_ky_timer_init(timer,timeout,parameter,time,flag);
}

ky_inline void _ky_timer_remove(ky_timer_t timer)
{
		for(int i=0;i<KY_TIMER_SKIP_LIST_LEVEL;i++)
		{
				ky_list_remove(&(timer->row[i]));
		}
}

//停止一个定时器
ky_err_t ky_timer_stop(ky_timer_t timer)
{
		register ky_base_t level;

    /* 只有active的定时器才能被停止，否则退出返回错误码 */
    if (!(timer->parent.flag & KY_TIMER_FLAG_ACTIVATED))
        return -KY_ERROR;

    /* 关中断 */
    level = rt_hw_interrupt_disable();

    /* 将定时器从定时器列表删除 */
    _ky_timer_remove(timer);

    /* 开中断 */
    rt_hw_interrupt_enable(level);

    /* 改变定时器的状态为非active */
    timer->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;

    return KY_EOK;
}

//根据cmd获取或设置定时器的参数
ky_err_t rt_timer_control(ky_timer_t timer, int cmd, void *arg)
{
    switch (cmd)
    {
    case KY_TIMER_CTRL_GET_TIME:                          
        *(ky_tick_t *)arg = timer->init_tick;
        break;

    case KY_TIMER_CTRL_SET_TIME:                           
        timer->init_tick = *(ky_tick_t *)arg;
        break;

    case KY_TIMER_CTRL_SET_ONESHOT:
        timer->parent.flag &= ~KY_TIMER_FLAG_PERIODIC;     
        break;

    case KY_TIMER_CTRL_SET_PERIODIC:
        timer->parent.flag |= KY_TIMER_FLAG_PERIODIC;     
        break;
    }

    return KY_EOK;
}

ky_err_t ky_timer_start(ky_timer_t timer)
{
		unsigned int row_lvl=0;
		ky_list_t *timer_list;
		register ky_base_t level;
		ky_list_t *row_head[KY_TIMER_SKIP_LIST_LEVEL];
		unsigned int tst_nr;
		static unsigned int random_nr;
	
		level = rt_hw_interrupt_disable();
		
		_ky_timer_remove(timer);
	
		//将定时器状态改为非active
		timer->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
	
		rt_hw_interrupt_enable(level);
	
		//定时器到期时间
		timer->timeout_tick = ky_time_get() + timer->init_tick;
	
}