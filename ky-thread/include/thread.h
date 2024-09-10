#ifndef ___THREAD_H__
#define ___THREAD_H__

#include "def.h"
#include "kyconfig.h"
#include "list.h"
#include "hw_settings.h"

//�߳̽ӿ�
ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size,
												ky_uint8_t priority);
												
ky_err_t ky_thread_delay(ky_tick_t tick);
												
ky_err_t ky_thread_startup(ky_thread_t thread);
												
void ky_thread_timeout(void *parameter);   												

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
int __ky_ffs(int value);

												
#endif
