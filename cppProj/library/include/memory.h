#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <fstream>
#include "property.h"
#include "legacy_string.h"
template<class T>
class Memory{
public:
	//basic data type function (char, short, int)
	void increase(T value, T interval, unsigned int size);
	void decrease(T value, T interval, unsigned int size);
	void toggle(T value, unsigned int size);
	void fill(T value, unsigned int size);
	void dump(uint32_t offset, uint32_t size, std::string &out);
	//bool compare(T* cmpPtr, uint32_t size, std::string &out, int actualOffset=0, int expectOffset=0, bool skipWhenErr=true);
    bool compare(T* cmpPtr, uint32_t size, std::string &out, int fromOffset=0, bool skipWhenErr=true);
	bool check(uint32_t offset, uint32_t size, T check, std::string &out, bool skipWhenErr=true);
	//structure type function
	int save_to(std::string fn, uint32_t size=sizeof(T));
	int load_from(std::string fn, uint32_t size=sizeof(T));	
	void *memory(void);
	Memory(T *ptr);
	Memory(unsigned int itemCnt);
	~Memory(void);
protected:
private:
	bool newByClass;
	//static const unsigned int dumpRowCnt;
	T *_ptr;
};

//template<class T>
//const unsigned int Memory<T>::dumpRowCnt=16/sizeof(T);

template<class T>
int Memory<T>::save_to(std::string fn, uint32_t size)
{
	int ret=-1;
	std::fstream *fs;
	fs = new std::fstream(fn.c_str(), std::fstream::out|std::fstream::trunc);
	if(fs != NULL)
	{
		fs->write((char *)_ptr, size);
		fs->flush();
        //force close this file
        //fs->close();
		ret = size;
		delete fs;
	}
	return ret;
}

template<class T>
int Memory<T>::load_from(std::string fn, uint32_t size)
{
	int ret=-1;
	std::fstream *fs;
	fs = new std::fstream(fn.c_str(), std::fstream::in);
	if(fs->good())
	{
		fs->read((char *)_ptr, size);		
		ret = size;
		delete fs;
	}		
	return ret;
}

template<class T>
void Memory<T>::fill(T value, unsigned int size)
{
	unsigned int repeat;
	int i=0;
	repeat = size/sizeof(T);
	while(repeat--)
	{
		_ptr[i++]=value;
	}	
}

template<class T>
void Memory<T>::increase(T value, T interval, unsigned int size)
{
	unsigned int repeat;
	int i=0;
	repeat = size/sizeof(T);
	while(repeat--)
	{
		_ptr[i++]=value;
		value+=interval;
	}	
}

template<class T>
void Memory<T>::toggle(T value, unsigned int size)
{
	unsigned int repeat;
	int i=0;
	repeat = size/sizeof(T);
	while(repeat--)
	{
		_ptr[i++]=(value^=(T)-1);
	}	
}

template<class T>
void Memory<T>::decrease(T value, T interval, unsigned int size)
{
	unsigned int repeat;
	int i=0;
	repeat = size/sizeof(T);
	while(repeat--)
	{
		_ptr[i++]=value;
		value-=interval;
	}	
}

template<class T>
void Memory<T>::dump(uint32_t offset, uint32_t size, std::string &out)
{
	int i=0;
	char *format[4]={(char *)"%02X ", (char *)"%04X ", (char *)"%08X ", (char *)"%016lX "};
	LegacyString lstr;
	
	unsigned int repeat;
	unsigned int dumpRowCnt, cnt=0;
	unsigned int sel=0;
	

	switch(sizeof(T))
	{
        //case 1:	 sel=3; dumpRowCnt = 16; break;
		case 2:	 sel=1; dumpRowCnt = 8; break;
		case 4:	 sel=2; dumpRowCnt = 4; break;
		case 8:	 sel=3; dumpRowCnt = 2; break;
		default: sel=0; dumpRowCnt = 1; break;
	}
	
	repeat = size/sizeof(T);
	out.clear();
	i=offset/sizeof(T);
	
	while(repeat--)
	{
		if(cnt==0)
		{			
			out += lstr.sprintf("0x%08X: ", offset);			
		}
		
		out += lstr.sprintf(format[sel], _ptr[i++]);

		if(++cnt==dumpRowCnt)
		{
			out += "\n";
			cnt = 0;
			offset += 16;
		}
	}	
}

template<class T>
bool Memory<T>::check(uint32_t offset, uint32_t size, T check, std::string &out, bool skipWhenErr)
{
	int i=0;
	bool match=true;
	LegacyString lstr;
	unsigned int repeat;	
	repeat = size/sizeof(T);
	out.clear();
	i=offset/sizeof(T);
	while(repeat--)
	{
		if(check!=_ptr[i])
		{
			out += lstr.sprintf("check mismatch [0x%08X] : expected=0x%08X, actual=0x%08X",
						i*sizeof(T), check, _ptr[i]);
					
			match=false;
						
			if(skipWhenErr)
			{				
				break;
			}
			else
			{
				out += "\n";
			}			
		}
		
		i++;
		
	}
	
	return match;
}
#if 0
template<class T>
bool Memory<T>::compare(T* cmpPtr, uint32_t size, std::string &out, int actualOffset, int expectOffset, bool skipWhenErr)
{
	unsigned int repeat;
	bool match=true;
	LegacyString lstr;
	//int i=fromOffset/sizeof(T);
	int actual=actualOffset/sizeof(T);
	int expect=expectOffset/sizeof(T);
	repeat = size/sizeof(T);

	while(repeat--)
	{
		if(_ptr[actual]!=cmpPtr[expect])
		{
			match=false;        
			out += lstr.sprintf("check mismatch [0x%08X] : expected=0x%08X, actual=0x%08X\n",
						actual*sizeof(T), cmpPtr[expect], _ptr[actual]);

			if(skipWhenErr)
			{
				break;
			}
			//else
			//{
			//	out += "\n";
			//}
		}
		//i++;
		actual++;
		expect++;		
	}
	return match;
}
#endif

template<class T>
bool Memory<T>::compare(T* cmpPtr, uint32_t size, std::string &out, int fromOffset, bool skipWhenErr)
{
	unsigned int repeat;
	bool match=true;
    LegacyString lstr;
	int i=fromOffset/sizeof(T);
	repeat = size/sizeof(T);

	while(repeat--)
	{
		if(_ptr[i]!=cmpPtr[i])
		{
			match=false;
            out+=lstr.sprintf("check mismatch [0x%08X] : expected=0x%08X, actual=0x%08X\n",
						i*sizeof(T), cmpPtr[i], _ptr[i]);
			if(skipWhenErr)
			{
				break;
			}
		}
		i++;
	}
	return match;
}

template<class T>
void *Memory<T>::memory(void)
{
	return _ptr;
}

template<class T>
Memory<T>::Memory(T *ptr)
{
	newByClass = false;
	_ptr = ptr;
}

template<class T>
Memory<T>::Memory(unsigned int itemCnt)
{
	newByClass = true;
	_ptr = new T[itemCnt];
}

template<class T>
Memory<T>::~Memory(void)
{
	if(newByClass)
	{
		delete _ptr;
	}
}
#endif //ndef __MEMORY_H__
