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

int ky_strlen(char* src)
{
		int len=0;
		while(*src++ != '\0')
		{
				len++;
		}
		return len;
}

//如果src1被包含于src2返回1，否则返回0
int ky_strcmp(char *src1,char *src2)
{
		while(*src1 == *src2)
		{
				src1++;
				src2++;
				if(*src1=='\0')
				{
						return 1;
				}
		}
		return 0;
}

void *ky_memmove(void *dest, const void *src, ky_ubase_t n)
{
    char *tmp = (char *)dest, *s = (char *)src;

    if (s < tmp && tmp < s + n)
    {
        tmp += n;
        s += n;

        while (n--)
            *(--tmp) = *(--s);
    }
    else
    {
        while (n--)
            *tmp++ = *s++;
    }

    return dest;
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
		printf("--------------------\r\n");
		printf("|      welcome     |\r\n");
		printf("|     ky-thread    |\r\n");
		printf("|    version  is   |\r\n");
		printf("|        V1.0      |\r\n");
		printf("--------------------\r\n");
}