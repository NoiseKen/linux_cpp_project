#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stropts.h>
#include <stdlib.h>
#include <errno.h>
#include "utils.h"


unsigned int 
search_free_slot_8( unsigned int status )
{
    unsigned int cnt;
    unsigned int bit;
    
    bit = INVALID_ITEM;
         
    if( status != 0xFF )
    {
        cnt = 8;
        while( cnt )
        {
            bit++;
            cnt=(status & (1<<bit))?cnt-1:0;
        }
    }
    
    return bit;
}

unsigned int 
search_free_slot_32( unsigned int slotStatus )
{
#if 0    
    unsigned int slotCount;
    unsigned int slotId;

    slotId = INVALID_ITEM;
    slotCount = 32;

    if( slotStatus != 0xFFFFFFFF )
    {            
        while( slotCount )
        {
            slotId++;
            if( slotStatus & (1<<slotId) ) 
            {
                slotCount--;
            }
            else
            {
                slotCount = 0; //find the un-used slot, trigger break condition
                //slotStatus |= 1<<slotId;    //mark the slot is used
            }
        }      
    }
    
    return slotId;
#else
    unsigned int bit=INVALID_ITEM;
    unsigned int byte;
    
    if(slotStatus!=INVALID_ITEM)
    {
		for( byte=0; byte<4; byte++ )
		{
			bit = search_free_slot_8( (slotStatus>>(byte<<3))&0xFF );
			if( bit != INVALID_ITEM )
			{
				//bit = byte * 8 + bit;
				bit = (byte<<3) + bit;
				byte = 4;
				//*slotStatus |= 1<<bit;            
			}    
		}
    }
    
    return bit;
#endif  
}

/*
void release_slot_32( unsigned int *slotStatus, unsigned int slotId )
{
    *slotStatus &= ~( 1 << slotId );   
}
*/

#if 1
/*
 * function	: get_ip4_str
 * 			  get ip4 string
 * parameter: [ip4]=pointer of string which want to sotre ip4 string
 * return	: none
 */
void 
get_ip4_str(char *ip4)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

	ioctl(fd, SIOCGIFADDR, &ifr);

	close(fd);

	sprintf(ip4, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}

void
endian_swap(char *src, int size)
{	
	int loop;
	char *f, *r, b;
	
	if(0==(size & 1))
	{
		f = &src[0];
		r = &src[size-1];
		loop=size>>1;
		while(loop--)
		{
			b = *f;
			*f = *r;
			*r = b;
			f++;
			r--;
		}
	}	
}

void
printf_128(char *int128)
{
	int loop;
	unsigned char *p;
	
	p = (unsigned char *)&int128[(128/8)-1];
	loop = 128/8;
	while(loop--)
	{		
		printf("%02X",*p--);		
	}
}

int 
system_call(const char *sys)
{
	int status;
	fflush(stdout);
	status = system(sys);
	fflush(stdout);
	return status;
}
/*
 * function	: aligned_malloc
 * 			  allocate an aligned memory block
 * parameter: [size] = byte size which want to allocate
 * 			  [pow] = power number of 2, align value will be 2^pow
 * return	: allocated memory block align with 2^pow
 * Notice	: please free memory use aligned_free
 */
void *
aligned_malloc(int size, int pow)
{
    unsigned int align;
    void *mem;
    void **ptr;
    align = 1<<pow;
    mem = malloc(size+align+sizeof(void*));    
    if(mem!=NULL)
		ptr = (void**)(((long)mem+align+sizeof(void*)) & ~(align-1));
	else
		ptr = NULL;
    ptr[-1] = mem;
    return ptr;
}

void 
aligned_free(void *ptr)
{
    free(((void**)ptr)[-1]);
}
#endif
