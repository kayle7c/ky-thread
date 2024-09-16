#include "thread.h"
#include "usart.h"

struct ky_thread ky_shell_thread;
ky_uint8_t ky_shell_thread_stack[KY_SHELL_STACK_SIZE];

int shell_getchar()
{
		int ch=-1;
	
		//如果有数据
		if(USART_GetITStatus(KY_SHELL_USART, USART_IT_RXNE) != RESET)
		{
				ch = USART_ReceiveData(KY_SHELL_USART);			
		}
		else
		{
				ky_thread_delay_ms(10);
		}
		return ch;
}

void shell_thread_entry()
{
		int ch;
	
		while(1)
		{
				ky_thread_delay_ms(10);
	//			ch=shell_getchar();
				if(ch<0)
				{
						continue;
				}
				printf("ch=%d\r\n",ch);
		}
	
}

//shell线程初始化
void ky_shell_init(void)
{
		ky_thread_init(&ky_shell_thread,
									"shell",
									shell_thread_entry,
									KY_NULL,
									&ky_shell_thread_stack[0],
									sizeof(ky_shell_thread_stack),
									KY_SHELL_PRIORITY,
									10);				
									
		ky_thread_startup(&ky_shell_thread);
		
	
}