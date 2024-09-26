#ifndef ___THREAD_H__
#define ___THREAD_H__

#include "sys.h"
#include "def.h"
#include "kyconfig.h"
#include "list.h"
#include "hw_settings.h"
#include "usart.h"
#include "ringbuffer.h"

//�߳̽ӿ�
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

//�ں˶���ӿ�
void ky_object_init(struct ky_object *object,
										enum ky_object_class_type type,
										const char *name);

//�������ӿ�
void ky_system_schedule_init(void);
void ky_system_schedule_start(void);
void ky_schedule(void);
void ky_schedule_insert_thread(struct ky_thread *thread);
void ky_schedule_remove_thread(struct ky_thread *thread);
										
//�����߳̽ӿ�
void ky_thread_idle_init(void);						

//�жϽӿ�
void ky_interrupt_enter(void);
void ky_interrupt_leave(void);	
										
//ʱ�ӽӿ�
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
										
//�ں˷������ӿ�
char *ky_strncpy(char *dst,const char*src,ky_base_t n);
int ky_strlen(char* src);
int ky_strcmp(char *src1,char *src2);
void *ky_memmove(void *dest, const void *src, ky_ubase_t n);
int __ky_ffs(int value);
void show_version(void);
									 
//����ӿ�
void kythread_startup(void);		

//shell�ӿ�
void ky_shell_init(void);									 

//sem�ź����ӿ�
ky_err_t ky_sem_release(ky_sem_t sem);
ky_err_t ky_sem_take(ky_sem_t sem,ky_int32_t time);
ky_sem_t ky_sem_creat(const char *name,
											ky_uint16_t value,
											ky_uint8_t flag);

#endif
