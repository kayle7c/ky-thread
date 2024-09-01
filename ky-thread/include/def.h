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
	

#define KY_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define KY_NULL                         (0)


struct ky_list_node
{
    struct ky_list_node *next;                        
    struct ky_list_node *prev;                     
};
typedef struct ky_list_node ky_list_t;                 

struct ky_thread
{
		char name[KY_NAME_MAX];
		ky_uint8_t type;
		ky_uint8_t flag;
		ky_list_t list;          //��������ڵ�
	
		void       *sp;                               
		void       *entry;                                  
		void       *parameter;                             
		void       *stack_addr;                             
		ky_uint32_t stack_size;    

		ky_list_t	 tlist;        //�߳�����ڵ�
	
		ky_ubase_t remaining_tick;
};
typedef struct ky_thread *ky_thread_t;

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

#endif


