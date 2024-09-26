#ifndef ___THREAD_H__
#define ___THREAD_H__

#include "sys.h"
#include "def.h"
#include "kyconfig.h"
#include "list.h"
#include "hw_settings.h"
#include "usart.h"
#include "ringbuffer.h"

//线程接口
ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size,
												ky_uint8_t priority,
												ky_uint32_t tick);
												
ky_err_t ky_thread_delay(ky_tick_t tick);										
void ky_thread_delay_ms(ky_tick_t ms);
void ky_thread_delay_s(ky_tick_t s);																							
ky_err_t ky_thread_startup(ky_thread_t thread);				
ky_err_t ky_thread_suspend(ky_thread_t thread);		
ky_err_t ky_thread_resume(ky_thread_t thread);												
ky_thread_t ky_thread_self(void);																	
void ky_thread_timeout(void *parameter);   		
ky_err_t ky_thread_yeild(void);												

//内核对象接口
void ky_object_init(struct ky_object *object,
										enum ky_object_class_type type,
										const char *name);

//调度器接口
void ky_system_schedule_init(void);
void ky_system_schedule_start(void);
void ky_schedule(void);
void ky_schedule_insert_thread(struct ky_thread *thread);
void ky_schedule_remove_thread(struct ky_thread *thread);
										
//空闲线程接口
void ky_thread_idle_init(void);						

//中断接口
void ky_interrupt_enter(void);
void ky_interrupt_leave(void);	
										
//时钟接口
void ky_timer_init(ky_timer_t timer,
									 const char *name,
									 void(*timeout)(void *parameter),
									 void *parameter,
									 ky_tick_t time,
									 ky_uint8_t flag);
void ky_system_timer_init();									 
void ky_tick_increase(void);					
ky_tick_t ky_tick_get(void);		
ky_err_t ky_timer_stop(ky_timer_t timer);
ky_err_t ky_timer_control(ky_timer_t timer, int cmd, void *arg);
ky_err_t ky_timer_start(ky_timer_t timer);
void ky_timer_check(void);												
										
//内核服务函数接口
char *ky_strncpy(char *dst,const char*src,ky_base_t n);
int ky_strlen(char* src);
int ky_strcmp(char *src1,char *src2);
void *ky_memmove(void *dest, const void *src, ky_ubase_t n);
int __ky_ffs(int value);
void show_version(void);
									 
//组件接口
void kythread_startup(void);		

//shell接口
void ky_shell_init(void);									 

//sem信号量接口
ky_err_t ky_sem_release(ky_sem_t sem);
ky_err_t ky_sem_take(ky_sem_t sem,ky_int32_t time);
ky_sem_t ky_sem_creat(const char *name,
											ky_uint16_t value,
											ky_uint8_t flag);

#endif
