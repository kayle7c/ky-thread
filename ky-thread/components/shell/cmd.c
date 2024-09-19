#include "thread.h"

extern ky_tick_t ky_tick;
extern ky_ubase_t ky_idletask_cnt;
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

void cmd_version(void* parameter)
{
		show_version();
}

void cmd_clear(void* parameter)
{
		printf("\033[2J\033[H");
}

void cmd_ps(void* parameter)
{
		struct ky_thread *thread;
		for(int i=0;i<KY_THREAD_PRIORITY_MAX;i++)
		{
				thread=ky_list_entry(ky_thread_priority_table[i].next,struct ky_thread,tlist);
//				if(ky_thread_priority_table[i])
//				{
//					
//						printf("%s  %d   ",);
//				}
				printf("%s\r\n",thread->name);
		}
	
}

void cmd_reboot(void* parameter)
{
		__set_FAULTMASK(1);//关闭所有中断
		NVIC_SystemReset();//复位函数	
}

void cmd_cpu(void* parameter)
{
		float use=1-(ky_idletask_cnt/ky_tick);
		printf("%d\r\n",ky_idletask_cnt);
		printf("%d\r\n",ky_tick);
		ky_idletask_cnt=0;
		ky_tick=0;
}