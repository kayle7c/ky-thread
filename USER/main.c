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

void delay (uint32_t count)
{
    for(; count!=0; count--);
}

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
        //ky_thread_delay(2); 		
				delay( 200 ); 
				flag2=0;
//				GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
        //ky_thread_delay(2);
				delay( 200 ); 
		}
}

void test3_thread_entry()
{
		while(1)
		{
				flag3=1;
//				GPIO_SetBits(GPIOB,GPIO_Pin_7); 
        //ky_thread_delay(3); 		
				delay( 200 ); 
				flag3=0;
//				GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
        //ky_thread_delay(3);
				delay( 200 ); 
		}
}

int main()
{
		LED_Init();
	
		kythread_startup();
	
	  //创建线程
	  ky_thread_init(&ky_test1_thread,
									"test1",
									test1_thread_entry,
									KY_NULL,
									&ky_test1_thread_stack[0],
									sizeof(ky_test1_thread_stack),
									2,
									4);
		ky_thread_startup(&ky_test1_thread);						
	
		ky_thread_init(&ky_test2_thread,
									"test2",
									test2_thread_entry,
									KY_NULL,
									&ky_test2_thread_stack[0],
									sizeof(ky_test2_thread_stack),
									3,
									2);
		ky_thread_startup(&ky_test2_thread);				

		ky_thread_init(&ky_test3_thread,
									"test3",
									test3_thread_entry,
									KY_NULL,
									&ky_test3_thread_stack[0],
									sizeof(ky_test3_thread_stack),
									3,
									3);
		ky_thread_startup(&ky_test3_thread);									
	
		ky_system_schedule_start();
}
			
void SysTick_Handler(void)
{
    ky_interrupt_enter();

    ky_tick_increase();

    ky_interrupt_leave();
}
