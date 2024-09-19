#include "thread.h"

extern ky_tick_t ky_tick;
extern ky_ubase_t ky_idletask_cnt;
extern ky_list_t ky_thread_priority_table[KY_THREAD_PRIORITY_MAX];

extern ky_thread_t thread_array[KY_THREAD_NUM_MAX];
extern ky_size_t thread_cnt;

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
		printf("thread    pri  status\r\n");
		printf("--------  ---  ------\r\n");
		for(int i=0;i<thread_cnt;i++)
		{
				printf("%-8s  %-5d",thread_array[i]->name,thread_array[i]->current_priority);
				if(thread_array[i]->stat==KY_THREAD_READY)					printf("ready\r\n");
				else if(thread_array[i]->stat==KY_THREAD_RUNNING)		printf("running\r\n");
				else if(thread_array[i]->stat==KY_THREAD_SUSPEND)		printf("suspend\r\n");
				else if(thread_array[i]->stat==KY_THREAD_INIT)			printf("init\r\n");
				else if(thread_array[i]->stat==KY_THREAD_CLOSE)			printf("close\r\n");
		}

}

void cmd_help(void* parameter)
{
		printf("ps              	-List threads in the system\r\n");
		printf("version         	-show KY-Thread version information\r\n");
		printf("clear         		-clear the terminal screen\r\n");
		printf("reboot         		-Reboot System\r\n");
		
}

void cmd_reboot(void* parameter)
{
		__set_FAULTMASK(1);//关闭所有中断
		NVIC_SystemReset();//复位函数	
}