#ifndef KY_CONFIG_H__
#define KY_CONFIG_H__

#define KY_THREAD_PRIORITY_MAX 			32

#define KY_ALIGN_SIZE								4

#define KY_NAME_MAX             		8

#define KY_TICK_PER_SECOND	        100

//shell
#define KY_USING_SHELL							0

#define KY_SHELL_USART              USART1
#define KY_SHELL_STACK_SIZE					1024
#define KY_SHELL_PRIORITY           10

#endif