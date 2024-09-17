#include "thread.h"

char *ky_strncpy(char *dst,const char*src,ky_base_t n)
{
		if (n != 0)
    {
        char *d = dst;
        const char *s = src;

        do
        {
            if ((*d++ = *s++) == 0)
            {
                while (--n != 0)
                    *d++ = 0;
                break;
            }
        } while (--n != 0);
    }

    return (dst);
}

int __ky_ffs(int value)
{
		if(value==0) return 0;
		int position=0;
		while((value&1)==0)
		{
				value >>= 1;
				position++;
		}	
		return position;
}

void show_version(void)
{
		printf("\r\n");
		printf("--------------------\r\n");
		printf("|      welcome     |\r\n");
		printf("|     ky-thread    |\r\n");
		printf("|    version  is   |\r\n");
		printf("|        V1.0      |\r\n");
		printf("--------------------\r\n");
		
}