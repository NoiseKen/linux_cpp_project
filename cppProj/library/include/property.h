#ifndef _PROPERTY_H_
#define _PROPERTY_H_
#include <stdlib.h>
#include <assert.h>

//----------- read/write property ----------------
template <class Container, class T>
class property_rw
{
typedef void (Container::*WriteHandler)(T);
typedef T (Container::*ReadHandler)(void);
private:
    Container* _owner; 	
    WriteHandler _set;    
    ReadHandler _get;
public:
	//set function
    T operator =(const T& value)
	{
		assert(_owner != NULL);
		assert(_set != NULL);
		(_owner->*_set)(value);
		return value;
	}
    //get function
    // internal type --
    operator T(void)
    {
		assert(_owner != NULL);
		assert(_get != NULL);
		return (_owner->*_get)();
    }

    property_rw(Container *c, ReadHandler r, WriteHandler w)
    {
		_owner = c;		
		_get = r;
		_set = w;
	}
};

//----------- read only property ----------------
template <class Container, class T>
class property_ro
{	
typedef T (Container::*ReadHandler)(void);
private:
    Container* _owner; 	
    ReadHandler _get;
public:
    //get function
    // internal type --
    operator T(void)
    {
		assert(_owner != NULL);
		assert(_get != NULL);
		return (_owner->*_get)();
    }

    property_ro(Container *c, ReadHandler r)
    {
		_owner = c;
		_get = r;
	}
};

//----------- write only property ----------------
template <class Container, class T>
class property_wo
{	
typedef void (Container::*WriteHandler)(T);
private:
    Container* _owner; 	
    WriteHandler _set;
public:
	//set function
    T operator =(const T& value)
	{
		assert(_owner != NULL);
		assert(_set != NULL);
		(_owner->*_set)(value);
		return value;
	}

    property_wo(Container *c, WriteHandler w)
    {
		_owner = c;
		_set = w;
	}
};

//----------- read/write properties ----------------
template <class Container, class T>
class properties_rw
{
typedef T& (Container::*AccessHandler)(unsigned int);
private:
    Container* _owner; 	
	AccessHandler _access;
public:
	//set function
    T& operator [](int i)
	{
		assert(_owner != NULL);
		assert(_access != NULL);
		return (_owner->*_access)(i);
	}
    
    properties_rw(Container *c, AccessHandler a)
    {
		_owner = c;		
		_access = a;
	}
};

//----------- read only properties ----------------
template <class Container, class T>
class properties_ro
{	
typedef T (Container::*AccessHandler)(int);
private:
    Container* _owner; 	
	AccessHandler _access;
public:
	//set function
    T operator [](int i)
	{
		assert(_owner != NULL);
		assert(_access != NULL);
		return (_owner->*_access)(i);
	}
    
    properties_ro(Container *c, AccessHandler a)
    {
		_owner = c;		
		_access = a;
	}
};

/*
//----------- write only property ----------------
template <class Container, class T>
class properties_wo
{	
typedef void (Container::*WriteHandler)(T);
private:
    Container* _owner; 	
    WriteHandler _set;
public:
	//set function
    operator [](int i, T value)
	{
		assert(_owner != NULL);
		assert(_set != NULL);
		(_owner->*_set)(i, value);
	}
	
    properties_wo(Container *c, WriteHandler w)
    {
		_owner = c;
		_set = w;
	}
};
*/
#endif //ndef _PROPERTY_H_
