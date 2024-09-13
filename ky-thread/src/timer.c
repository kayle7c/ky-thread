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
		
		timer->timeout_func=timeout;
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
ky_err_t ky_timer_control(ky_timer_t timer, int cmd, void *arg)
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
		timer->timeout_tick = ky_tick_get() + timer->init_tick;
	
		level = rt_hw_interrupt_disable();
	
		timer_list = ky_timer_list;  
		
		row_head[0]  = &timer_list[0];  
		
		for (row_lvl = 0; row_lvl < KY_TIMER_SKIP_LIST_LEVEL; row_lvl++)
		{	
				//当定时器列表中没有定时器不执行
				for (; row_head[row_lvl] != timer_list[row_lvl].prev; row_head[row_lvl]  = row_head[row_lvl]->next)
				{
						struct ky_timer *t;
					
						ky_list_t *p=row_head[row_lvl]->next;
						
						t = ky_list_entry(p,                 
                              struct ky_timer,  
                              row[row_lvl]);
						//如果两个定时器延时时间相同，就找下一个
						if((t->timeout_tick - timer->timeout_tick) == 0)
						{
								continue;
						}							
						else if((t->timeout_tick - timer->timeout_tick)<KY_TICK_MAX/2) //比大小
						{
								break;		
								//要插入的定时器比当前定时器时间长，就在这里退出然后insert_after该定时器
								//要插入的定时器比当前定时器时间短，就进for循环row_head[row_lvl]  = row_head[row_lvl]->next，寻找下一个定时器节点
						}
				}
				if (row_lvl != KY_TIMER_SKIP_LIST_LEVEL - 1)
        {
						row_head[row_lvl + 1] = row_head[row_lvl] + 1;
        }
		}
		
		//记录启动了多少定时器
		random_nr++;
		tst_nr = random_nr;
		
		//在上面的for循环中已经找到当前定时器要插入的位置，现在直接在row[0]之后插入即可
		ky_list_insert_after(row_head[KY_TIMER_SKIP_LIST_LEVEL - 1],      
                        &(timer->row[KY_TIMER_SKIP_LIST_LEVEL - 1])); 
		
		//设置定时器为激活态
		timer->parent.flag |= KY_TIMER_FLAG_ACTIVATED;
		
		rt_hw_interrupt_enable(level);
		
		return -KY_EOK;
}

void ky_timer_check(void)
{
		struct ky_timer *t;
		ky_tick_t current_tick;
		register ky_base_t level;
	
		current_tick=ky_tick_get();
	
		level = rt_hw_interrupt_disable();
	
		//如果定时器列表不为空，就扫描
		while(!ky_list_isempty(&ky_timer_list[KY_TIMER_SKIP_LIST_LEVEL - 1]))
		{
				//获取第一个定时器的地址，也就是剩余延时时间最少的
				t = ky_list_entry(ky_timer_list[KY_TIMER_SKIP_LIST_LEVEL - 1].next,   
													struct ky_timer,                                     
													row[KY_TIMER_SKIP_LIST_LEVEL - 1]); 
													
				//第一个定时器的延时时间已经到了
				if((current_tick - t->timeout_tick)<KY_TICK_MAX/2)
				{
						//移除第一个定时器
						_ky_timer_remove(t);

						//调用超时函数
						t->timeout_func(t->parameter);

						current_tick=ky_tick_get();
					
						//如果是周期定时器
						if((t->parent.flag & KY_TIMER_FLAG_PERIODIC) &&(t->parent.flag & KY_TIMER_FLAG_ACTIVATED))
						{
								//重新启动定时器
								t->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
								ky_timer_start(t);							
						}
						//单次定时器
						else
						{
								//
								t->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
							
						}
				}
				else
				{
						break;
				}
		}
	  rt_hw_interrupt_enable(level);    
}



