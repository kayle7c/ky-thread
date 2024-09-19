#ifndef __SHELL_H
#define __SHELL_H
#include "thread.h"

enum input_stat
{
		WAIT_NORMAL_KEY,   
		WAIT_DIR_KEY,			
		WAIT_FUNC_KEY,		
};
struct cmd_node
{
		char* name;
		int cmd_length;
	
		void       (*entry)(void *parameter);
		void       *parameter;
};
typedef struct cmd_node ky_cmd_t; 
//shell�ṹ��
struct ky_shell
{
		enum input_stat stat;
		
		char cmd[KY_SHELL_LENGSH];	//ָ������
	
		char* history[KY_SHELL_HISORY_MAX];
		ky_size_t history_cnt;
		ky_size_t current_history;
	
		ky_size_t position;     	//��ǰ�г���
		ky_size_t curpos;     		//����λ��
};

#endif