#ifndef __SHELL_H
#define __SHELL_H

#include "thread.h"



enum input_stat
{
		WAIT_NORMAL_KEY,   
		WAIT_DIR_KEY,			
		WAIT_FUNC_KEY,		
};

//shell结构体
struct ky_shell
{
		enum input_stat stat;
		
		char cmd[KY_SHELL_LENGSH];	//指令内容
	
		ky_size_t position;     	//当前行长度
		ky_size_t curpos;     		//光标的位置
};


#endif