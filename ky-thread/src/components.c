#include "thread.h"

void kythread_startup(void)
{
		//���ж�
		rt_hw_interrupt_disable();
	
		SysTick_Config( SystemCoreClock / 100 );
	
		//��ʼ��������
		ky_system_schedule_init();
	
		//��ʼ�������߳�
		ky_thread_idle_init();
	
}