#include "thread.h"


enum rt_object_info_type
{
    KY_Object_Info_Thread = 0,                         /* 对象是线程 */
#ifdef KY_USING_SEMAPHORE
    KY_Object_Info_Semaphore,                          /* 对象是信号量 */
#endif
#ifdef KY_USING_MUTEX
    KY_Object_Info_Mutex,                              /* 对象是互斥量 */
#endif
#ifdef KY_USING_EVENT
    KY_Object_Info_Event,                              /* 对象是事件 */
#endif
#ifdef KY_USING_MAILBOX
    KY_Object_Info_MailBox,                            /* 对象是邮箱 */
#endif
#ifdef KY_USING_MESSAGEQUEUE
    KY_Object_Info_MessageQueue,                       /* 对象是消息队列 */
#endif
#ifdef KY_USING_MEMHEAP
    KY_Object_Info_MemHeap,                            /* 对象是内存堆 */
#endif
#ifdef KY_USING_MEMPOOL
    KY_Object_Info_MemPool,                            /* 对象是内存池 */
#endif
#ifdef KY_USING_DEVICE
    KY_Object_Info_Device,                             /* 对象是设备 */
#endif
    KY_Object_Info_Timer,                              /* 对象是定时器 */
#ifdef KY_USING_MODULE
    KY_Object_Info_Module,                             /* 对象是模块 */
#endif
    KY_Object_Info_Unknown,                            /* 对象未知 */
};

#define _OBJ_CONTAINER_LIST_INIT(c)     \
    {&(ky_object_container[c].object_list), &(ky_object_container[c].object_list)}
		
static struct ky_object_information ky_object_container[KY_Object_Info_Unknown] =
{
    /* 初始化对象容器 - 线程 */
    {
        KY_Object_Class_Thread, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Thread), 
        sizeof(struct ky_thread)
    },
		
#ifdef KY_USING_SEMAPHORE
    /* 初始化对象容器 - 信号量 */
    {
        KY_Object_Class_Semaphore, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Semaphore), 
        sizeof(struct ky_semaphore)
    },
#endif		
		
#ifdef KY_USING_MUTEX
    /* 初始化对象容器 - 互斥量 */
    {
        KY_Object_Class_Mutex, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Mutex), 
        sizeof(struct ky_mutex)
    },
#endif		
		
#ifdef KY_USING_EVENT
    /* 初始化对象容器 - 事件 */
    {
        KY_Object_Class_Event, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Event), 
        sizeof(struct ky_event)
    },
#endif
		
#ifdef KY_USING_MAILBOX
    /* 初始化对象容器 - 邮箱 */
    {
        KY_Object_Class_MailBox, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MailBox), 
        sizeof(struct ky_mailbox)
    },
#endif	
		
#ifdef KY_USING_MESSAGEQUEUE
    /* 初始化对象容器 - 消息队列 */
    {
        KY_Object_Class_MessageQueue, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MessageQueue), 
        sizeof(struct ky_messagequeue)
    },
#endif		
		
#ifdef KY_USING_MEMHEAP
    /* 初始化对象容器 - 内存堆 */
    {
        KY_Object_Class_MemHeap, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MemHeap), 
        sizeof(struct ky_memheap)
    },
#endif		
		
#ifdef KY_USING_MEMPOOL
    /* 初始化对象容器 - 内存池 */
    {
        KY_Object_Class_MemPool, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MemPool), 
        sizeof(struct ky_mempool)
    },
#endif		
	
#ifdef KY_USING_DEVICE
    /* 初始化对象容器 - 设备 */
    {
        KY_Object_Class_Device, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Device), sizeof(struct ky_device)},
#endif
    /* 初始化对象容器 - 定时器 */
    /*
    {
        KY_Object_Class_Timer, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Timer), 
        sizeof(struct ky_timer)
    },
    */
#ifdef KY_USING_MODULE
    /* 初始化对象容器 - 模块 */
    {
        KY_Object_Class_Module, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Module), 
        sizeof(struct ky_module)
    },
#endif

		
};