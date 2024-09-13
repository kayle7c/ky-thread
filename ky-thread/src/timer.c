#include "thread.h"

//��ʱ���б�
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
		
		//����Ϊ�Ǽ���̬
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

//��ʼ����ʱ��
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

//ֹͣһ����ʱ��
ky_err_t ky_timer_stop(ky_timer_t timer)
{
		register ky_base_t level;

    /* ֻ��active�Ķ�ʱ�����ܱ�ֹͣ�������˳����ش����� */
    if (!(timer->parent.flag & KY_TIMER_FLAG_ACTIVATED))
        return -KY_ERROR;

    /* ���ж� */
    level = rt_hw_interrupt_disable();

    /* ����ʱ���Ӷ�ʱ���б�ɾ�� */
    _ky_timer_remove(timer);

    /* ���ж� */
    rt_hw_interrupt_enable(level);

    /* �ı䶨ʱ����״̬Ϊ��active */
    timer->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;

    return KY_EOK;
}

//����cmd��ȡ�����ö�ʱ���Ĳ���
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
	
		//����ʱ��״̬��Ϊ��active
		timer->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
	
		rt_hw_interrupt_enable(level);
	
		//��ʱ������ʱ��
		timer->timeout_tick = ky_tick_get() + timer->init_tick;
	
		level = rt_hw_interrupt_disable();
	
		timer_list = ky_timer_list;  
		
		row_head[0]  = &timer_list[0];  
		
		for (row_lvl = 0; row_lvl < KY_TIMER_SKIP_LIST_LEVEL; row_lvl++)
		{	
				//����ʱ���б���û�ж�ʱ����ִ��
				for (; row_head[row_lvl] != timer_list[row_lvl].prev; row_head[row_lvl]  = row_head[row_lvl]->next)
				{
						struct ky_timer *t;
					
						ky_list_t *p=row_head[row_lvl]->next;
						
						t = ky_list_entry(p,                 
                              struct ky_timer,  
                              row[row_lvl]);
						//���������ʱ����ʱʱ����ͬ��������һ��
						if((t->timeout_tick - timer->timeout_tick) == 0)
						{
								continue;
						}							
						else if((t->timeout_tick - timer->timeout_tick)<KY_TICK_MAX/2) //�ȴ�С
						{
								break;		
								//Ҫ����Ķ�ʱ���ȵ�ǰ��ʱ��ʱ�䳤�����������˳�Ȼ��insert_after�ö�ʱ��
								//Ҫ����Ķ�ʱ���ȵ�ǰ��ʱ��ʱ��̣��ͽ�forѭ��row_head[row_lvl]  = row_head[row_lvl]->next��Ѱ����һ����ʱ���ڵ�
						}
				}
				if (row_lvl != KY_TIMER_SKIP_LIST_LEVEL - 1)
        {
						row_head[row_lvl + 1] = row_head[row_lvl] + 1;
        }
		}
		
		//��¼�����˶��ٶ�ʱ��
		random_nr++;
		tst_nr = random_nr;
		
		//�������forѭ�����Ѿ��ҵ���ǰ��ʱ��Ҫ�����λ�ã�����ֱ����row[0]֮����뼴��
		ky_list_insert_after(row_head[KY_TIMER_SKIP_LIST_LEVEL - 1],      
                        &(timer->row[KY_TIMER_SKIP_LIST_LEVEL - 1])); 
		
		//���ö�ʱ��Ϊ����̬
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
	
		//�����ʱ���б�Ϊ�գ���ɨ��
		while(!ky_list_isempty(&ky_timer_list[KY_TIMER_SKIP_LIST_LEVEL - 1]))
		{
				//��ȡ��һ����ʱ���ĵ�ַ��Ҳ����ʣ����ʱʱ�����ٵ�
				t = ky_list_entry(ky_timer_list[KY_TIMER_SKIP_LIST_LEVEL - 1].next,   
													struct ky_timer,                                     
													row[KY_TIMER_SKIP_LIST_LEVEL - 1]); 
													
				//��һ����ʱ������ʱʱ���Ѿ�����
				if((current_tick - t->timeout_tick)<KY_TICK_MAX/2)
				{
						//�Ƴ���һ����ʱ��
						_ky_timer_remove(t);

						//���ó�ʱ����
						t->timeout_func(t->parameter);

						current_tick=ky_tick_get();
					
						//��������ڶ�ʱ��
						if((t->parent.flag & KY_TIMER_FLAG_PERIODIC) &&(t->parent.flag & KY_TIMER_FLAG_ACTIVATED))
						{
								//����������ʱ��
								t->parent.flag &= ~KY_TIMER_FLAG_ACTIVATED;
								ky_timer_start(t);							
						}
						//���ζ�ʱ��
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



