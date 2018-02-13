#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <QFile>
#include <QTextStream>
#include "property.h"
#include "html_string.h"
#include "debug.h"
template<class T>
class Memory{
public:
	//basic data type function (char, short, int)
	void increase(T value, T interval, unsigned int size);
	void decrease(T value, T interval, unsigned int size);
	void toggle(T value, unsigned int size);
	void fill(T value, unsigned int size);
	void dump(uint32_t offset, uint32_t size, HtmlString &out);
	int compare(T* cmpPtr, uint32_t size, QString &out);	
	//structure type function
	int save_to(QString filePath, uint32_t size=sizeof(T));
	int load_from(QString filePath, uint32_t size=sizeof(T));	
	property_ro<class Memory, T *> memory;
	Memory(T *ptr);
	Memory(unsigned int itemCnt);
	~Memory(void);
protected:
private:
	bool newByClass;
	T * _f_get_memory(void);
	static const unsigned int dumpRowCnt;
	T *_ptr;
};

template<class T>
const unsigned int Memory<T>::dumpRowCnt=16/sizeof(T);

template<class T>
int Memory<T>::save_to(QString filePath, uint32_t size)
{
	QFile *fn;
	int byteWritten=0;
	fn = new QFile(filePath);
	if (fn->open(QIODevice::WriteOnly))
	{
		QDataStream *ds;
		ds = new QDataStream(fn);
		//byteWritten = ds->writeRawData((char *)_ptr, sizeof(T));
		byteWritten = ds->writeRawData((char *)_ptr, size);
		delete ds;
	}		
	delete fn;
	return byteWritten;
}

template<class T>
int Memory<T>::load_from(QString filePath, uint32_t size)
{
	QFile *fn;
	int byteRead=0;
	fn = new QFile(filePath);
	if (fn->open(QIODevice::ReadOnly))
	{
		QDataStream *ds;
		ds = new QDataStream(fn);
		byteRead = ds->readRawData((char *)_ptr, size);
		delete ds;
	}		
	delete fn;
	return byteRead;
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
void Memory<T>::dump(uint32_t offset, uint32_t size, HtmlString &out)
{
	QString d;
	int i=0;
	HtmlString::ColorStr cstr;
	unsigned int repeat;
	unsigned int cnt=0;
	bool odd=true;

	repeat = size/sizeof(T);
	
	while(repeat--)
	{
		if(cnt==0)
		{			
			cstr.color = (odd)?HtmlString::colorDefault:HtmlString::colorCyan;
			cstr.string = d.sprintf("0x%08X: ", offset);
			odd ^= 1;			
		}
		
		if(sizeof(T)==1)
		{
			cstr.string += d.sprintf("%02X ", _ptr[i++]);
		}
		else
		if(sizeof(T)==2)
		{
			cstr.string += d.sprintf("%04X ", _ptr[i++]);
		}
		else
		if(sizeof(T)==4)
		{
			cstr.string += d.sprintf("%08X ", _ptr[i++]);
		}
		
		if(++cnt==dumpRowCnt)
		{
			cstr.string += "\n";
			out << cstr;
			cnt = 0;
			offset += 16;
		}
	}	
}

template<class T>
int Memory<T>::compare(T* cmpPtr, uint32_t size, QString &out)
{
	unsigned int repeat;
	int i=0;
	int result = 0;
	repeat = size/sizeof(T);
	
	while(repeat--)
	{
		if(_ptr[i]!=cmpPtr[i])
		{
			out += QString().sprintf("CMP fail @ offset 0x%016lX, expected=0x%08X, actually=0x%08X\n", i*sizeof(T), _ptr[i], cmpPtr[i]);
			result++;
		}
		i++;
	}
	
	return -result;
}

template<class T>
T * Memory<T>::_f_get_memory(void)
{
	return _ptr;
}

template<class T>
Memory<T>::Memory(T *ptr) : memory(this, &Memory<T>::_f_get_memory)
{
	newByClass = false;
	_ptr = ptr;
}

template<class T>
Memory<T>::Memory(unsigned int itemCnt) : memory(this, &Memory<T>::_f_get_memory)
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
