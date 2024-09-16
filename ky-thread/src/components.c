#include "thread.h"
#include "led.h"
#include "usart.h"

void kythread_startup(void)
{
		LED_Init();
		
		rt_hw_interrupt_disable();
	
		SysTick_Config( SystemCoreClock / KY_TICK_PER_SECOND );
	
		//��ʱ����ʼ��
		ky_system_timer_init();
	
		//��ʼ��������
		ky_system_schedule_init();		
	
		//��ʼ�������߳�
		ky_thread_idle_init();
				
}