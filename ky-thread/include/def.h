#ifndef ___DEF_H__
#define ___DEF_H__

#include "kyconfig.h"

typedef signed   long         ky_int32_t;
typedef unsigned   long       ky_uint32_t;
typedef unsigned char         ky_uint8_t; 

typedef long									ky_base_t; 
typedef unsigned long         ky_ubase_t;


typedef ky_base_t         		ky_err_t;
typedef ky_ubase_t            ky_size_t;
typedef ky_uint32_t           ky_tick_t;

#define ky_inline             static __inline
#define ALIGN(n)              __attribute__((aligned(n)))
	
#define KY_EOK                          0               /**< There is no error */
#define KY_ERROR                        1               /**< A generic error happens */
#define KY_ETIMEOUT                     2               /**< Timed out */
#define KY_EFULL                        3               /**< The resource is full */
#define KY_EEMPTY                       4               /**< The resource is empty */
#define KY_ENOMEM                       5               /**< No memory */
#define KY_ENOSYS                       6               /**< No system */
#define KY_EBUSY                        7               /**< Busy */
#define KY_EIO                          8               /**< IO error */
#define KY_EINTR                        9               /**< Interrupted system call */
#define KY_EINVAL                       10              /**< Invalid argument */
	
#define KY_THREAD_INIT                  0x00                /* ��ʼ̬ */
#define KY_THREAD_READY                 0x01                /* ����̬ */
#define KY_THREAD_SUSPEND               0x02                /* ����̬ */
#define KY_THREAD_RUNNING               0x03                /* ����̬ */
#define KY_THREAD_BLOCK                 KY_THREAD_SUSPEND   /* ����̬ */
#define KY_THREAD_CLOSE                 0x04                /* �ر�̬ */
#define KY_THREAD_STAT_MASK             0x0f

#define KY_THREAD_STAT_SIGNAL           0x10
#define KY_THREAD_STAT_SIGNAL_READY     (KY_THREAD_STAT_SIGNAL | KY_THREAD_READY)
#define KY_THREAD_STAT_SIGNAL_SUSPEND   0x20
#define KY_THREAD_STAT_SIGNAL_MASK      0xf0

#define KY_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define KY_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define KY_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */
#define KY_TICK_MAX                     KY_UINT32_MAX   /**< Maxium number of tick */


#define KY_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define KY_NULL                         (0)

//����ṹ��
struct ky_list_node
{
    struct ky_list_node *next;                        
    struct ky_list_node *prev;                     
};
typedef struct ky_list_node ky_list_t;           

//����ṹ��
struct ky_object
{
		char name[KY_NAME_MAX];
		ky_uint8_t type;
		ky_uint8_t flag;
		ky_list_t list;
	
};
typedef struct ky_object *ky_object_t;

enum ky_object_class_type
{
    KY_Object_Class_Thread = 0,       /* �������߳� */
    KY_Object_Class_Semaphore,        /* �������ź��� */
    KY_Object_Class_Mutex,            /* �����ǻ����� */
    KY_Object_Class_Event,            /* �������¼� */
    KY_Object_Class_MailBox,          /* ���������� */
    KY_Object_Class_MessageQueue,     /* ��������Ϣ���� */
    KY_Object_Class_MemHeap,          /* �������ڴ�� */
    KY_Object_Class_MemPool,          /* �������ڴ�� */
    KY_Object_Class_Device,           /* �������豸 */
    KY_Object_Class_Timer,            /* �����Ƕ�ʱ�� */
    KY_Object_Class_Module,           /* ������ģ�� */
    KY_Object_Class_Unknown,          /* ����δ֪ */
    KY_Object_Class_Static = 0x80     /* �����Ǿ�̬���� */
};

struct ky_object_information
{
    enum ky_object_class_type type;//  (1) /* �������� */
    ky_list_t object_list;//           (2) /* �����б�ڵ�ͷ */
    ky_size_t object_size;//           (3) /* �����С */
};

//ʱ�� ��ʱ����
#ifndef KY_TIMER_SKIP_LIST_LEVEL
#define KY_TIMER_SKIP_LIST_LEVEL          1
#endif

#define KY_TIMER_FLAG_DEACTIVATED       0x0     /* ��ʱ��û�м��� */
#define KY_TIMER_FLAG_ACTIVATED         0x1     /* ��ʱ���Ѿ����� */
#define KY_TIMER_FLAG_ONE_SHOT          0x0     /* ���ζ�ʱ */
#define KY_TIMER_FLAG_PERIODIC          0x2     /* ���ڶ�ʱ */

#define KY_TIMER_FLAG_HARD_TIMER        0x0     /* Ӳ����ʱ������ʱ���ص������� tick isr�е��� */

#define KY_TIMER_FLAG_SOFT_TIMER        0x4     /* �����ʱ������ʱ���ص������ڶ�ʱ���߳��е��� */��

#define KY_TIMER_CTRL_SET_TIME          0x0     /* ���ö�ʱ����ʱʱ�� */
#define KY_TIMER_CTRL_GET_TIME          0x1     /* ��ȡ��ʱ����ʱʱ�� */
#define KY_TIMER_CTRL_SET_ONESHOT       0x2     /* �޸Ķ�ʱ��Ϊһ�ζ�ʱ */
#define KY_TIMER_CTRL_SET_PERIODIC      0x3     /* �޸Ķ�ʱ��Ϊ���ڶ�ʱ */

//��ʱ���ṹ��
struct ky_timer
{
		struct ky_object parent;
		ky_list_t row[KY_TIMER_SKIP_LIST_LEVEL]; //��ʱ������ڵ�
		void (*timeout_func)(void *parameter);     //��ʱ����
		void *parameter;                         //�β�
		ky_tick_t init_tick;                     //��ʱ����Ҫ��ʱ��ʱ��
	  ky_tick_t timeout_tick;        					 //��ʱ��ʵ�ʳ�ʱ��ʱ��
		
};
typedef struct ky_timer *ky_timer_t;


//�߳̽ṹ��
struct ky_thread
{
		char* name;
		ky_uint8_t type;
		ky_uint8_t flag;
		ky_list_t list;          //��������ڵ�
	
		ky_list_t	 tlist;        //�߳�����ڵ�
	
		void       *sp;                               
		void       *entry;                                  
		void       *parameter;                             
		void       *stack_addr;                             
		ky_uint32_t stack_size;    
	
		ky_ubase_t init_tick;  			//��ʼʱ��Ƭ
		ky_ubase_t remaining_tick;  //ʣ��ʱ��Ƭ
	
		ky_size_t  run_time;
	
		ky_uint8_t  current_priority;     
    ky_uint8_t  init_priority;       
    ky_uint32_t number_mask;          

    ky_err_t    error;             
    ky_uint8_t  stat;         

		struct ky_timer thread_timer; //���õ��̶߳�ʱ��
};
typedef struct ky_thread *ky_thread_t;
#endif


