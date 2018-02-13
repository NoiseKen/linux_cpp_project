#ifndef __APP_VER_H__
#define __APP_VER_H__


#define VER(major,minor,build)	(((major&0xFF)<<24)|((minor&0xFF)<<16)|(build &0xFFFF))

#define MAJOR_VERSION	0
#define MINOR_VERSION	0
//BUILD_VERSION will auto increase do not modify it!!!!
#define BUILD_VERSI   1
//BUILD_VERSION will auto increase do not modify it!!!!

#define APP_VER		VER(MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION)

#define TO_STR(s)	(#s)


#endif//ndef __APP_VER_H__
