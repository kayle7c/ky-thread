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
	
#define KY_THREAD_INIT                  0x00                /* 初始态 */
#define KY_THREAD_READY                 0x01                /* 就绪态 */
#define KY_THREAD_SUSPEND               0x02                /* 挂起态 */
#define KY_THREAD_RUNNING               0x03                /* 运行态 */
#define KY_THREAD_BLOCK                 KY_THREAD_SUSPEND   /* 阻塞态 */
#define KY_THREAD_CLOSE                 0x04                /* 关闭态 */
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

//链表结构体
struct ky_list_node
{
    struct ky_list_node *next;                        
    struct ky_list_node *prev;                     
};
typedef struct ky_list_node ky_list_t;           

//对象结构体
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
    KY_Object_Class_Thread = 0,       /* 对象是线程 */
    KY_Object_Class_Semaphore,        /* 对象是信号量 */
    KY_Object_Class_Mutex,            /* 对象是互斥量 */
    KY_Object_Class_Event,            /* 对象是事件 */
    KY_Object_Class_MailBox,          /* 对象是邮箱 */
    KY_Object_Class_MessageQueue,     /* 对象是消息队列 */
    KY_Object_Class_MemHeap,          /* 对象是内存堆 */
    KY_Object_Class_MemPool,          /* 对象是内存池 */
    KY_Object_Class_Device,           /* 对象是设备 */
    KY_Object_Class_Timer,            /* 对象是定时器 */
    KY_Object_Class_Module,           /* 对象是模块 */
    KY_Object_Class_Unknown,          /* 对象未知 */
    KY_Object_Class_Static = 0x80     /* 对象是静态对象 */
};

struct ky_object_information
{
    enum ky_object_class_type type;//  (1) /* 对象类型 */
    ky_list_t object_list;//           (2) /* 对象列表节点头 */
    ky_size_t object_size;//           (3) /* 对象大小 */
};

//时钟 定时器宏
#ifndef KY_TIMER_SKIP_LIST_LEVEL
#define KY_TIMER_SKIP_LIST_LEVEL          1
#endif

#define KY_TIMER_FLAG_DEACTIVATED       0x0     /* 定时器没有激活 */
#define KY_TIMER_FLAG_ACTIVATED         0x1     /* 定时器已经激活 */
#define KY_TIMER_FLAG_ONE_SHOT          0x0     /* 单次定时 */
#define KY_TIMER_FLAG_PERIODIC          0x2     /* 周期定时 */

#define KY_TIMER_FLAG_HARD_TIMER        0x0     /* 硬件定时器，定时器回调函数在 tick isr中调用 */

#define KY_TIMER_FLAG_SOFT_TIMER        0x4     /* 软件定时器，定时器回调函数在定时器线程中调用 */、

#define KY_TIMER_CTRL_SET_TIME          0x0     /* 设置定时器定时时间 */
#define KY_TIMER_CTRL_GET_TIME          0x1     /* 获取定时器定时时间 */
#define KY_TIMER_CTRL_SET_ONESHOT       0x2     /* 修改定时器为一次定时 */
#define KY_TIMER_CTRL_SET_PERIODIC      0x3     /* 修改定时器为周期定时 */

//定时器结构体
struct ky_timer
{
		struct ky_object parent;
		ky_list_t row[KY_TIMER_SKIP_LIST_LEVEL]; //定时器自身节点
		void (*timeout_func)(void *parameter);     //超时函数
		void *parameter;                         //形参
		ky_tick_t init_tick;                     //定时器需要延时的时间
	  ky_tick_t timeout_tick;        					 //定时器实际超时的时间
		
};
typedef struct ky_timer *ky_timer_t;


//线程结构体
struct ky_thread
{
		char* name;
		ky_uint8_t type;
		ky_uint8_t flag;
		ky_list_t list;          //对象链表节点
	
		ky_list_t	 tlist;        //线程链表节点
	
		void       *sp;                               
		void       *entry;                                  
		void       *parameter;                             
		void       *stack_addr;                             
		ky_uint32_t stack_size;    
	
		ky_ubase_t init_tick;  			//初始时间片
		ky_ubase_t remaining_tick;  //剩余时间片
	
		ky_size_t  run_time;
	
		ky_uint8_t  current_priority;     
    ky_uint8_t  init_priority;       
    ky_uint32_t number_mask;          

    ky_err_t    error;             
    ky_uint8_t  stat;         

		struct ky_timer thread_timer; //内置的线程定时器
};
typedef struct ky_thread *ky_thread_t;
#endif


