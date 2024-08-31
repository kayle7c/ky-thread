#include "thread.h"


enum rt_object_info_type
{
    KY_Object_Info_Thread = 0,                         /* �������߳� */
#ifdef KY_USING_SEMAPHORE
    KY_Object_Info_Semaphore,                          /* �������ź��� */
#endif
#ifdef KY_USING_MUTEX
    KY_Object_Info_Mutex,                              /* �����ǻ����� */
#endif
#ifdef KY_USING_EVENT
    KY_Object_Info_Event,                              /* �������¼� */
#endif
#ifdef KY_USING_MAILBOX
    KY_Object_Info_MailBox,                            /* ���������� */
#endif
#ifdef KY_USING_MESSAGEQUEUE
    KY_Object_Info_MessageQueue,                       /* ��������Ϣ���� */
#endif
#ifdef KY_USING_MEMHEAP
    KY_Object_Info_MemHeap,                            /* �������ڴ�� */
#endif
#ifdef KY_USING_MEMPOOL
    KY_Object_Info_MemPool,                            /* �������ڴ�� */
#endif
#ifdef KY_USING_DEVICE
    KY_Object_Info_Device,                             /* �������豸 */
#endif
    KY_Object_Info_Timer,                              /* �����Ƕ�ʱ�� */
#ifdef KY_USING_MODULE
    KY_Object_Info_Module,                             /* ������ģ�� */
#endif
    KY_Object_Info_Unknown,                            /* ����δ֪ */
};

#define _OBJ_CONTAINER_LIST_INIT(c)     \
    {&(ky_object_container[c].object_list), &(ky_object_container[c].object_list)}
		
static struct ky_object_information ky_object_container[KY_Object_Info_Unknown] =
{
    /* ��ʼ���������� - �߳� */
    {
        KY_Object_Class_Thread, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Thread), 
        sizeof(struct ky_thread)
    },
		
#ifdef KY_USING_SEMAPHORE
    /* ��ʼ���������� - �ź��� */
    {
        KY_Object_Class_Semaphore, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Semaphore), 
        sizeof(struct ky_semaphore)
    },
#endif		
		
#ifdef KY_USING_MUTEX
    /* ��ʼ���������� - ������ */
    {
        KY_Object_Class_Mutex, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Mutex), 
        sizeof(struct ky_mutex)
    },
#endif		
		
#ifdef KY_USING_EVENT
    /* ��ʼ���������� - �¼� */
    {
        KY_Object_Class_Event, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Event), 
        sizeof(struct ky_event)
    },
#endif
		
#ifdef KY_USING_MAILBOX
    /* ��ʼ���������� - ���� */
    {
        KY_Object_Class_MailBox, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MailBox), 
        sizeof(struct ky_mailbox)
    },
#endif	
		
#ifdef KY_USING_MESSAGEQUEUE
    /* ��ʼ���������� - ��Ϣ���� */
    {
        KY_Object_Class_MessageQueue, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MessageQueue), 
        sizeof(struct ky_messagequeue)
    },
#endif		
		
#ifdef KY_USING_MEMHEAP
    /* ��ʼ���������� - �ڴ�� */
    {
        KY_Object_Class_MemHeap, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MemHeap), 
        sizeof(struct ky_memheap)
    },
#endif		
		
#ifdef KY_USING_MEMPOOL
    /* ��ʼ���������� - �ڴ�� */
    {
        KY_Object_Class_MemPool, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_MemPool), 
        sizeof(struct ky_mempool)
    },
#endif		
	
#ifdef KY_USING_DEVICE
    /* ��ʼ���������� - �豸 */
    {
        KY_Object_Class_Device, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Device), sizeof(struct ky_device)},
#endif
    /* ��ʼ���������� - ��ʱ�� */
    /*
    {
        KY_Object_Class_Timer, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Timer), 
        sizeof(struct ky_timer)
    },
    */
#ifdef KY_USING_MODULE
    /* ��ʼ���������� - ģ�� */
    {
        KY_Object_Class_Module, 
        _OBJ_CONTAINER_LIST_INIT(KY_Object_Info_Module), 
        sizeof(struct ky_module)
    },
#endif

		
};