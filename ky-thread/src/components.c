#include "thread.h"
#include "led.h"
#include "usart.h"

void kythread_startup(void)
{
		LED_Init();
		
		uart_init(115200);
		
		rt_hw_interrupt_disable();
	
		SysTick_Config( SystemCoreClock / 1000 );
	
		//定时器初始化
		ky_system_timer_init();
	
		//初始化调度器
		ky_system_schedule_init();
	
#if KY_USING_SHELL
		ky_shell_init();

#endif	
	
		//初始化空闲线程
		ky_thread_idle_init();
			
	
}