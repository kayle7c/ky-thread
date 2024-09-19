#include "thread.h"
#include "led.h"
#include "cmd.h"

ky_uint8_t flag1;
ky_uint8_t flag2;
ky_uint8_t flag3;

extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

struct ky_thread ky_test1_thread;
struct ky_thread ky_test2_thread;

ALIGN(KY_ALIGN_SIZE)

ky_uint8_t ky_test1_thread_stack[512];
ky_uint8_t ky_test2_thread_stack[512];


void test1_thread_entry()
{
		while(1)
		{
				flag1=1;
				GPIO_SetBits(GPIOB,GPIO_Pin_8); 
				ky_thread_delay_s(1); 		
				flag1=0;
				GPIO_ResetBits(GPIOB,GPIO_Pin_8); 
        ky_thread_delay_s(1);
		}
}

void test2_thread_entry()
{
		while(1)
		{
				flag2=1;
				GPIO_SetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay_ms(500); 		
				flag2=0;
				GPIO_ResetBits(GPIOB,GPIO_Pin_7); 
        ky_thread_delay_ms(500);
		}
}


int main()
{
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
	
		ky_system_schedule_start();
}
			
void SysTick_Handler(void)
{
    ky_interrupt_enter();

    ky_tick_increase();

    ky_interrupt_leave();
}
