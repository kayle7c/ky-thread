#ifndef __CMD_H
#define __CMD_H
#include "thread.h"
void cmd_version(void* parameter);
void cmd_clear(void* parameter);
void cmd_ps(void* parameter);
void cmd_reboot(void* parameter);
void cmd_help(void* parameter);

#endif