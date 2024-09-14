#include "thread.h"

void kythread_startup(void)
{
		//关中断
		rt_hw_interrupt_disable();
	
		SysTick_Config( SystemCoreClock / 100 );
	
		//初始化调度器
		ky_system_schedule_init();
	
		//初始化空闲线程
		ky_thread_idle_init();
	
}