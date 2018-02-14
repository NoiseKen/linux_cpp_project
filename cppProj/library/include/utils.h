#ifndef _UTILS_H_
#define _UTILS_H_

#include <linux/types.h>
#include <sys/time.h>
#include <endian.h>
#include <cstdint>

#ifdef __CHECKER__
#define __force       __attribute__((force))
#else
#define __force
#endif

#define cpu_to_le16(x) \
	((__force __le16)htole16(x))
#define cpu_to_le32(x) \
	((__force __le32)htole32(x))
#define cpu_to_le64(x) \
	((__force __le64)htole64(x))

#define le16_to_cpu(x) \
	le16toh((__force __u16)(x))
#define le32_to_cpu(x) \
	le32toh((__force __u32)(x))
#define le64_to_cpu(x) \
	le64toh((__force __u64)(x))

extern __inline__ uint32_t getticks(void)
{
struct timeval  tv;
gettimeofday(&tv, NULL);
return (uint32_t)(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
}

/*
extern __inline__ uint64_t getticks(void)
{
     unsigned a, d;
     asm volatile("rdtsc" : "=a" (a), "=d" (d));
     return (((uint64_t)a) | (((uint64_t)d) << 32));
}
*/
//#ifndef TO_STR
#define TO_STR(s)	(#s)
//#endif

#define ALIGN(v,n)    (((v)+((n)-1))&~((n)-1))

#define BIT_BUNDLE(cnt) ((1<<(cnt))-1)

#define BIT_MASK(msb,lsb) (BIT_BUNDLE((msb)-(lsb)+1)<<(lsb))

#define mBIT(b)		(1U<<(b))


#define MINIMUM(a,b)	(a>b)?b:a
#define MAXIMUM(a,b)	(a>b)?a:b

#define mLastBit(r)  ((r)&~((r)-1))
#define mPartCopy(dest, from, part) (((dest^from)&~(part))^from)
#define mZoneMake(bitH, bitL) (((0x01L<<(bitH+1))-1)&(-1<<(bitL)))
#define mZoneCopy(dest, data, bitH, bitL) mPartCopy(dest, (data<<bitL), mZoneMake(bitH, bitL) )
#define mEC16(data) (((data>>8)&0x00FF)|((data<<8)&0xFF00))
#define mEC32(data) (((data >> 24) & 0x000000ff) |\
                     ((data << 24) & 0xff000000) |\
                     ((data >> 8) & 0x0000ff00) | \
                     ((data << 8) & 0x00ff0000))

#define REG64(addr)   (*(volatile unsigned logn long *)(addr))
#define REG32(addr)   (*(volatile unsigned int *)(addr))
#define REG16(addr)   (*(volatile unsigned short *)(addr))
#define REG8(addr)   (*(volatile unsigned char *)(addr))

#define SWAP32(val) ( (((val)>>24)&0x000000FF)\
                     |(((val)>> 8)&0x0000FF00)\
                     |(((val)<< 8)&0x00FF0000)\
                     |(((val)<<24)&0xFF000000) )
#define SWAP16(val) ( (((val)>> 8)&0x000000FF)\
                     |(((val)<< 8)&0x0000FF00) )

#define SWAP(a,b)	(a^=b^=a^=b)

#define INVALID_ITEM    0xFFFFFFFF                     

#ifndef __EOS
#define __EOS		0x00
#endif

#define __EOM		(TStr)0xFFFFFFFF

typedef unsigned long long  u64;
typedef unsigned int        u32;
typedef unsigned short      u16;
typedef unsigned char       u8;

typedef char *TStr;
typedef TStr const *TStrList;
typedef TStrList *TStrListSet;

typedef void(*GeneralTask)(void);
typedef void * TPurePoint;

#define APP_HISTORY_FILE_PATH	"/var/log/nxgn_history.log"
#define APP_CONFIG_FILE_NAME	"config.dat"


extern unsigned int search_free_slot_8(unsigned int slotStatus);
extern unsigned int search_free_slot_32(unsigned int slotStatus);
extern void release_slot_32(unsigned int *slotStatus, unsigned int slotId);

extern void get_ip4_str(char *ip4);
//extern char *get_user_input(const char *desc, char *inBuf, int sizeOfBuf);
extern int system_call(const char *sys);
extern void *aligned_malloc(int size, int pow);
extern void aligned_free(void *ptr);
extern void endian_swap(char *src, int size);
extern void printf_128(char *int128);
extern inline uint8_t mmio_read8(void *addr){ return *(uint8_t *)addr; }
extern inline uint16_t mmio_read16(void *addr){	__le16 *p = (__le16 *)addr; return le16_to_cpu(*p); }
extern inline uint32_t mmio_read32(void *addr){	__le32 *p = (__le32 *)addr; return le32_to_cpu(*p); }
extern inline __u64 mmio_read64(void *addr){__le32 *p = (__le32*)addr;return le32_to_cpu(*p) | ((uint64_t)le32_to_cpu(*(p + 1)) << 32);}


extern inline void mmio_write8(void *addr, uint8_t val){ *(uint8_t *)addr=val;}
extern inline void mmio_write16(void *addr, uint16_t val){	*(__le16 *)addr=val;}
extern inline void mmio_write32(void *addr, uint32_t val){ *(__le32 *)addr=val;}

#endif//ndef _UTILS_H_
