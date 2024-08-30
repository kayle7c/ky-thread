#ifndef ___DEF_H__
#define ___DEF_H__

typedef signed   long         ky_int32_t;
typedef unsigned   long       ky_uint32_t;
typedef unsigned char         ky_uint8_t; 

typedef long									ky_base_t; 

typedef ky_base_t         		ky_err_t;

#define ky_inline             static __inline
	
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
	void       *sp;                               
  void       *entry;                                  
  void       *parameter;                             
  void       *stack_addr;                             
  ky_uint32_t stack_size;    

	ky_list_t	 tlist;
};
typedef struct ky_thread *ky_thread_t;

#endif


