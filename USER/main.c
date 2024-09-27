#include "thread.h"
#include "led.h"
#include "cmd.h"

ky_uint8_t flag1;
ky_uint8_t flag2;
ky_uint8_t flag3;

extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

struct ky_thread ky_test1_thread;
struct ky_thread ky_test2_thread;
struct ky_thread ky_test3_thread;
struct ky_thread ky_test4_thread;

ALIGN(KY_ALIGN_SIZE)

ky_uint8_t ky_test1_thread_stack[512];
ky_uint8_t ky_test2_thread_stack[512];
ky_uint8_t ky_test3_thread_stack[512];
ky_uint8_t ky_test4_thread_stack[512];

static ky_sem_t sem = KY_NULL;


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

void test3_thread_entry()
{
		ky_err_t t=KY_EOK;
		while(1)
		{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
				{
						t=ky_sem_take(sem,0);
						if(t==KY_EOK)
						{
								printf("success\r\n");
						}
						else
						{
							printf("failed\r\n");
						}
				}
        ky_thread_delay_ms(200);
		}
}

void test4_thread_entry()
{
		ky_err_t t=KY_EOK;
		while(1)
		{
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
				{
						t=ky_sem_release(sem);
						if(t==KY_EOK)
						{
								printf("release\r\n");
						}
						else
						{
							printf("cant release\r\n");
						}
				}
        ky_thread_delay_ms(200);
		}
}



int main()
{
		kythread_startup();
	
		sem=ky_sem_creat("test_sem",5,KY_IPC_FLAG_FIFO);
	
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
									4,
									2);
		ky_thread_startup(&ky_test3_thread);					

		ky_thread_init(&ky_test4_thread,
									"test4",
									test4_thread_entry,
									KY_NULL,
									&ky_test4_thread_stack[0],
									sizeof(ky_test4_thread_stack),
									5,
									2);
		ky_thread_startup(&ky_test4_thread);									
	
		ky_system_schedule_start();
}
			
void SysTick_Handler(void)
{
    ky_interrupt_enter();

    ky_tick_increase();

    ky_interrupt_leave();
}
