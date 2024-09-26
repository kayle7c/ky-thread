#ifndef KY_CONFIG_H__
#define KY_CONFIG_H__

#define KY_THREAD_PRIORITY_MAX 			32
#define KY_THREAD_NUM_MAX						100

#define KY_ALIGN_SIZE								4

#define KY_NAME_MAX             		8

#define KY_TICK_PER_SECOND	        1000

#define KY_USING_SHELL              1
#define KY_SHELL_PRIORITY           10
#define KY_SHELL_STACK_SIZE         1024
#define KY_SHELL_USART              USART1
#define KY_SHELL_LENGSH							50
#define KY_SHELL_HISORY_MAX         30
#define KY_CMD_NUM_MAX              200

#define KY_USING_SEMAPHORE          1
#define KY_SEM_NUM_MAX							50

#endif