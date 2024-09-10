#include "sys.h"
#include "delay.h"
#include "usart.h"// RX->9 TX->A10
#include "thread.h"
#include "led.h"

ky_uint8_t flag1;
ky_uint8_t flag2;
ky_uint8_t flag3;

extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

struct ky_thread ky_test1_thread;
struct ky_thread ky_test2_thread;
struct ky_thread ky_test3_thread;

ALIGN(KY_ALIGN_SIZE)

ky_uint8_t ky_test1_thread_stack[512];
ky_uint8_t ky_test2_thread_stack[512];
ky_uint8_t ky_test3_thread_stack[512];

void test1_thread_entry()
{
		while(1)
		{
				flag1=1;
//				GPIO_SetBits(GPIOB,GPIO_Pin_8); 
				ky_thread_delay(4); 		
				flag1=0;
//				GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
        ky_thread_delay(4);
		}
}

void test2_thread_entry()
{
		while(1)
		{
				flag2=1;
//				GPIO_SetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay(2); 		
				flag2=0;
//				GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay(2);
		}
}

void test3_thread_entry()
{
		while(1)
		{
				flag3=1;
//				GPIO_SetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay(3); 		
				flag3=0;
//				GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay(3);
		}
}

int main()
{
		LED_Init();
	
		rt_hw_interrupt_disable();   //先关中断，防止还没有初始化好就开始调度
	
		SysTick_Config( SystemCoreClock / 100 );
	
	  //初始化调度器
		ky_system_schedule_init();
		
		//初始化空闲线程
		ky_thread_idle_init();
	
	  //创建线程
	  ky_thread_init(&ky_test1_thread,
									"test1",
									test1_thread_entry,
									KY_NULL,
									&ky_test1_thread_stack[0],
									sizeof(ky_test1_thread_stack),
									2);
		ky_thread_startup(&ky_test1_thread);						
	
		ky_thread_init(&ky_test2_thread,
									"test2",
									test2_thread_entry,
									KY_NULL,
									&ky_test2_thread_stack[0],
									sizeof(ky_test2_thread_stack),
									3);
		ky_thread_startup(&ky_test2_thread);				

		ky_thread_init(&ky_test3_thread,
									"test3",
									test3_thread_entry,
									KY_NULL,
									&ky_test3_thread_stack[0],
									sizeof(ky_test3_thread_stack),
									4);
		ky_thread_startup(&ky_test3_thread);									
	
		ky_system_schedule_start();
}
			
void SysTick_Handler(void)
{
    ky_interrupt_enter();

    ky_tick_increase();

    ky_interrupt_leave();
}
