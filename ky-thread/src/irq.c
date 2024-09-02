#include "thread.h"
\
volatile ky_uint8_t ky_interrupt_cnt;  //ÖĞ¶Ï¼ÆÊıÆ÷

void ky_interrupt_enter(void)
{
		ky_base_t level;
	
		level=rt_hw_interrupt_disable();
	
		ky_interrupt_cnt++;
	
		rt_hw_interrupt_enable(level);
}

void ky_interrupt_leave(void)
{
		ky_base_t level;
	
		level=rt_hw_interrupt_disable();
	
		ky_interrupt_cnt--;
	
		rt_hw_interrupt_enable(level);
} 