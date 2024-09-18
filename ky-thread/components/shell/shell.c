#include "thread.h"
#include "usart.h"
#include "shell.h"
#include "cmd.h"

struct ky_thread ky_shell_thread;
ky_uint8_t ky_shell_thread_stack[KY_SHELL_STACK_SIZE];

static ky_cmd_t cmd_table[KY_CMD_NUM_MAX];
ky_size_t current_cmd_num=0;

struct ky_shell my_shell;
struct ky_shell *shell = &my_shell;

int shell_getchar()
{
		int ch=-1;
	
		//如果有数据
		if(USART_GetFlagStatus(KY_SHELL_USART, USART_FLAG_RXNE) != RESET)
		{
				ch = USART_ReceiveData(KY_SHELL_USART);			
		}
		
		else
		{
				if(USART_GetFlagStatus(KY_SHELL_USART, USART_FLAG_ORE) != RESET)
				{
						printf("error");
				}
				ky_thread_delay_ms(100);
		}
		return ch;
}

ky_ubase_t my_strcmp(char *cmd,char *sys_cmd)
{
		while(*cmd == *sys_cmd)
		{
				if(*cmd=='\0')
				{
						return 1;
				}
				cmd++;
				sys_cmd++;
		}
		return 0;
}

void shell_match(char *cmd,ky_size_t length)	
{
#if 0
		char clear[5]="clear";
		int clearflag=1;
		char version[6]="version";
		
		for(int i=0;i<5;i++)
		{
				if(cmd[i]!=clear[i])
				{
						clearflag=0;
				}
		}
		if(clearflag==1)
		{
			printf("\033[2J\033[H");
			printf("ky />");
			return ;
		}
		for(int i=0;i<6;i++)
		{
				if(cmd[i]!=version[i])
				{
						printf("\r\n");
						printf("cmd not found!");
						printf("\r\n");
						printf("ky />");	
						return; 
				}
		}
		show_version();
		printf("ky />");	
#else 
		for(int i=0;i<current_cmd_num;i++)
		{
				if(length == cmd_table[i].cmd_length)
				{
						if(my_strcmp(cmd,cmd_table[i].name))
						{
								cmd_table[i].entry(cmd_table[i].parameter);
								printf("ky />");
								return ;
						}
				}
		}
		printf("\r\n");
		printf("cmd not found!");
		printf("\r\n");
		printf("ky />");
#endif 
}

void shell_thread_entry()
{
		int ch;
	
		//up key  : 0x1b 0x5b 0x41
    //down key: 0x1b 0x5b 0x42
    //right key:0x1b 0x5b 0x43
    //left key: 0x1b 0x5b 0x44
		printf("ky />");
		while(1)
		{
				ch=shell_getchar();
				if(ch<0)
				{
						continue;
				}
//***************************************************************
//*********************处理方向键********************************
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
								printf("2");
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
//***************************************************************
//*********************处理tab键*********************************
				if(ch=='\t')
				{
							
					
				}
//***************************************************************
//*********************处理退格键********************************
				if(ch==0x08)
				{
						//光标位置在开头
						if(shell->curpos==0)
						{
								continue;
						}
						shell->position--;
						shell->curpos--;
						printf("\b \b");   //光标回退
						shell->cmd[shell->curpos]=0;
						continue;
				}
//***************************************************************
//*********************处理回车键********************************				
				if(ch=='\r'||ch=='\n')
				{
						shell_match(shell->cmd,shell->position);					
						memset(shell->cmd,0,sizeof(shell->cmd));
						shell->position=0;
						shell->curpos=0;
						continue;
				}
////***************************************************************
////*********************处理常规字符******************************			
				shell->cmd[shell->position]=ch;
				printf("%c",ch);
				shell->position++;
				shell->curpos++;
		}
}

void cmd_list_add(char* name,
									 ky_size_t length,
									 void (*entry)(void *parameter),
									 void *parameter)
{
			cmd_table[current_cmd_num].name=name;
			cmd_table[current_cmd_num].cmd_length=length;
			cmd_table[current_cmd_num].entry=entry;
			cmd_table[current_cmd_num].parameter=parameter;
	
			current_cmd_num++;
//		register ky_base_t temp;
//		ky_cmd_t cmd;
//		ky_list_init(&cmd.cmd_list);
//	
//		cmd.name=name;
//		cmd.cmd_length=length;
//		cmd.entry=entry;
//		cmd.parameter=parameter;
//	
//		temp=rt_hw_interrupt_disable();
//		ky_list_insert_before(&(cmd.cmd_list),&(cmd_head));
//		rt_hw_interrupt_enable(temp);
}

void system_cmd_init()
{
		cmd_list_add("version",7,cmd_version,KY_NULL);
		cmd_list_add("clear",5,cmd_clear,KY_NULL);
		cmd_list_add("cpu",3,cmd_cpu,KY_NULL);
		cmd_list_add("ps",2,cmd_ps,KY_NULL);
		
}

//shell线程初始化
void ky_shell_init(void)
{
		system_cmd_init();
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