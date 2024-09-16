#include "thread.h"


/* 用于存储上一个线程的栈的sp的指针 */
ky_uint32_t rt_interrupt_from_thread;

/* 用于存储下一个将要运行的线程的栈的sp的指针 */
ky_uint32_t rt_interrupt_to_thread;

/* PendSV中断服务函数执行标志 */
ky_uint32_t rt_thread_switch_interrupt_flag;

struct exception_stack_frame
{
    ky_uint32_t r0;
    ky_uint32_t r1;
    ky_uint32_t r2;
    ky_uint32_t r3;
    ky_uint32_t r12;
    ky_uint32_t lr;
    ky_uint32_t pc;
    ky_uint32_t psr;
};

struct stack_frame
{
    ky_uint32_t r4;
    ky_uint32_t r5;
    ky_uint32_t r6;
    ky_uint32_t r7;
    ky_uint32_t r8;
    ky_uint32_t r9;
    ky_uint32_t r10;
    ky_uint32_t r11;

    struct exception_stack_frame exception_stack_frame;
};

ky_uint8_t *ky_hw_stack_init(void *tentry,
													void *parameter,
													ky_uint8_t *stack_addr)
{
		struct stack_frame *stack_frame;
		ky_uint8_t *stk;
	
		stk = stack_addr+sizeof(ky_uint8_t);
		stk = (ky_uint8_t *)KY_ALIGN_DOWN((ky_uint32_t)stk, 8);	
		stk -= sizeof(struct stack_frame);    //调整栈顶位置
	
		stack_frame=(struct stack_frame *)stk;
	
		for(int i=0;i<sizeof(struct stack_frame)/sizeof(ky_uint32_t);i++)
		{
				((ky_uint32_t*)stack_frame)[i]=0xdeadbeef;
		}
	
		//构造栈
		stack_frame->exception_stack_frame.r0  = (unsigned long)parameter; /* r0 : argument */
    stack_frame->exception_stack_frame.r1  = 0;                        /* r1 */
    stack_frame->exception_stack_frame.r2  = 0;                        /* r2 */
    stack_frame->exception_stack_frame.r3  = 0;                        /* r3 */
    stack_frame->exception_stack_frame.r12 = 0;                        /* r12 */
    stack_frame->exception_stack_frame.lr  = 0;                        /* lr */
    stack_frame->exception_stack_frame.pc  = (unsigned long)tentry;    /* entry point, pc */
    stack_frame->exception_stack_frame.psr = 0x01000000L;              /* PSR */
	
		return stk;//返回栈指针
}