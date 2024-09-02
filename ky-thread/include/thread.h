#ifndef ___THREAD_H__
#define ___THREAD_H__

#include "def.h"
#include "kyconfig.h"
#include "list.h"
#include "hw_settings.h"

//线程接口
ky_err_t ky_thread_init(struct ky_thread *thread,
												const char *name,
												void (*entry)(void *parameter),
												void *parameter,
												void *stack_start,
												ky_uint32_t stack_size);
												
void ky_thread_delay(ky_tick_t tick);

//内核对象接口
void ky_object_init(struct ky_object *object,
										enum ky_object_class_type type,
										const char *name);

//调度器接口
void ky_system_scheduler_init(void);
void ky_system_scheduler_start(void);
void ky_scheduler(void);
										
//空闲线程接口
void ky_thread_idle_init(void);						

//中断接口
void ky_interrupt_enter(void);
void ky_interrupt_leave(void);	
										
//时钟接口
void ky_tick_increase(void);										
										
										
//内核服务函数接口
char *ky_strncpy(char *dst,const char*src,ky_base_t n);										

												
#endif
