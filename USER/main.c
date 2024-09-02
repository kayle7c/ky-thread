#include "sys.h"
#include "delay.h"
#include "usart.h"// RX->9 TX->A10
#include "thread.h"

ky_uint8_t flag1;
ky_uint8_t flag2;

extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

struct ky_thread ky_test1_thread;
struct ky_thread ky_test2_thread;

ky_uint8_t ky_test1_thread_stack[512];
ky_uint8_t ky_test2_thread_stack[512];

void test1_thread_entry()
{
		while(1)
		{
			flag1=1;
			ky_thread_delay(2);
			flag1=0;
			ky_thread_delay(2);
		}
}

void test2_thread_entry()
{
		while(1)
		{
			flag2=1;
			ky_thread_delay(2);
			flag2=0;
			ky_thread_delay(2);
		}
}

int main()
{
		rt_hw_interrupt_disable();   //�ȹ��жϣ���ֹ��û�г�ʼ���þͿ�ʼ����
	
		SysTick_Config( SystemCoreClock / KY_TICK_PER_SECOND );
	
	  //��ʼ��������
		ky_system_scheduler_init();
		
		//��ʼ�������߳�
		ky_thread_idle_init();
	
	  //�����߳�
	  ky_thread_init(&ky_test1_thread,
									"test1",
									test1_thread_entry,
									KY_NULL,
									&ky_test1_thread_stack[0],
									sizeof(ky_test1_thread_stack));
	  //�������õ��̼߳��뵽��������
		ky_list_insert_before(&(ky_thread_priority_table[0]),&(ky_test1_thread.tlist));
	
		ky_thread_init(&ky_test2_thread,
									"test2",
									test2_thread_entry,
									KY_NULL,
									&ky_test2_thread_stack[0],
									sizeof(ky_test2_thread_stack));
		ky_list_insert_before(&(ky_thread_priority_table[1]),&(ky_test2_thread.tlist));
	
		ky_system_scheduler_start();
}
			
void SysTick_Handler(void)
{
    ky_interrupt_enter();

    ky_tick_increase();

    ky_interrupt_leave();
}

