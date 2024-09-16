#include "thread.h"
#include "usart.h"

struct ky_thread ky_shell_thread;
ky_uint8_t ky_shell_thread_stack[KY_SHELL_STACK_SIZE];

struct ky_shell *shell;

int shell_getchar()
{
		int ch=-1;
	
		//如果有数据
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
		{
				ch = USART_ReceiveData(KY_SHELL_USART);			
		}
		else
		{
				ky_thread_delay_ms(100);
		}
		return ch;
}

void shell_thread_entry()
{
		int ch;
	
		//up key  : 0x1b 0x5b 0x41
    //down key: 0x1b 0x5b 0x42
    //right key:0x1b 0x5b 0x43
    //left key: 0x1b 0x5b 0x44
		
		while(1)
		{
				ch=shell_getchar();
				if(ch<0)
				{
						continue;
				}
				printf("ch=%d\r\n",ch);
				//处理方向键
				if(ch==0x1b)
				{
						shell->stat=WAIT_DIR_KEY;
						printf("1");
						continue;
				}
				else if(shell->stat==WAIT_DIR_KEY)
				{
						if(ch==0x5b)
						{
								shell->stat=WAIT_FUNC_KEY;
								continue;
						}
						shell->stat=WAIT_NORMAL_KEY;
				}
				else if(shell->stat==WAIT_FUNC_KEY)
				{
						shell->stat=WAIT_NORMAL_KEY;
						if(ch==0x41)
						{
								printf("up");
								continue;
						}
					
				}
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