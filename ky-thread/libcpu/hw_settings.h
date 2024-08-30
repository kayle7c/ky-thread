#ifndef ___HW_SETTINGS_H__
#define ___HW_SETTINGS_H__

#include "def.h"

ky_uint8_t *ky_hw_stack_init(void *tentry,
													void *paramter,
													ky_uint8_t *stack_addr);
													
void rt_hw_context_switch(ky_uint32_t from, ky_uint32_t to);
void rt_hw_context_switch_to(ky_uint32_t to);
void rt_hw_context_switch_interrupt(ky_uint32_t from, ky_uint32_t to);
													

#endif
