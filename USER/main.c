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
			delay_ms(100);
			flag1=0;
			delay_ms(100);
			
			ky_scheduler();
		}
}

void test2_thread_entry()
{
		while(1)
		{
			flag2=1;
			delay_ms(100);
			flag2=0;
			delay_ms(100);
			
			ky_scheduler();
		}
}

int main()
{
	
		delay_init();
	
	  //初始化调度器
		ky_system_scheduler_init();
	
	  //创建线程
	  ky_thread_init(&ky_test1_thread,
									"test1",
									test1_thread_entry,
									KY_NULL,
									&ky_test1_thread_stack[0],
									sizeof(ky_test1_thread_stack));
	  //将创建好的线程加入到就绪队列
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
			


