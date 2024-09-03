#ifndef ___LIST_H__
#define ___LIST_H__

#include "def.h"

/* 已知一个结构体里面的成员的地址，反推出该结构体的首地址 */
#define ky_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
			
#define ky_list_entry(node, type, member) \
    ky_container_of(node, type, member)

ky_inline void ky_list_init(ky_list_t *l)
{
		l->next=l->prev=l;
}

ky_inline void ky_list_insert_after(ky_list_t *l,ky_list_t *n)
{
		n->next=l->next;
		n->prev=l;
		l->next->prev=n;
		l->next=n;
}

ky_inline void ky_list_insert_before(ky_list_t *l,ky_list_t *n)
{
		n->next=l;
		n->prev=l->prev;
		l->prev->next=n;
		l->prev=n;
}

ky_inline void ky_list_remove(ky_list_t *n)
{
		n->next->prev=n->prev;
		n->prev->next=n->next;
		n->next=n->prev=n;
}

ky_inline int ky_list_isempty(const ky_list_t *l)
{
		return l->next==l;
}

#endif
