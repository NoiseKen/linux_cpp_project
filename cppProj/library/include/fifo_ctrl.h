#ifndef _FIFO_CTRL_H_
#define _FIFO_CTRL_H_
#include "property.h"

/*
 * Class name : FIFOCtrl
 * Description: Generic FIFO controller
 * Usage :
 *      FIFOCtrl<int> *fifo;				//declare a int type fifo
 * 		fifo = new FIFOCtrl<int>(10);		//create and set fifo depth 10
 * 		*fifo << 1 << 2 << 3;				//push 1, 2, 3 to fifo
 * 		*fifl >> p[0] >> p[1] >> p[2];		//pop element from fifo
 */
template <class T>
class FIFOCtrl
{
private:
	T *_pool;	
    unsigned int _depth;    
    unsigned int _head;
    unsigned int _tail;
    bool _f_check_empty(void);
    bool _f_check_full(void);
protected:
	unsigned int _threshold;
public:
    //push operator
    class FIFOCtrl<T>& operator <<(T in);
    //pull operator
    class FIFOCtrl<T>& operator >>(T& out);
    //useful properties
    property_ro<FIFOCtrl ,bool> bFull;
    property_ro<FIFOCtrl ,bool> bEmpty;
    //override event function (for inheritance)
    virtual void on_threshold(void){};
    virtual void on_full(void){};
    //constructor
    FIFOCtrl(unsigned int n);
    //destructor
    ~FIFOCtrl(void);
};

template <class T>
class FIFOCtrl<T>& FIFOCtrl<T>::operator >>(T &out)
{
	if(!bEmpty)
	{
		out = _pool[_head];
		_head = (_head+1)%_depth;
	}
		
	return *this;
}

template <class T>
class FIFOCtrl<T>& FIFOCtrl<T>::operator <<(T in)
{
	if(!bFull)
	{
		_pool[_tail]=in;
		_tail = (_tail+1)%_depth;
	}
	
	if(((_tail+_depth-_head)%_depth)>_threshold)
	{
		on_threshold();
	}
	
	return *this;
}

template <class T>
bool FIFOCtrl<T>::_f_check_full(void)
{
	return (_head!=(_tail+1)%_depth)?false:true;
}

template <class T>
bool FIFOCtrl<T>::_f_check_empty(void)
{
	return (_head==_tail)?true:false;
}

template <class T>
FIFOCtrl<T>::FIFOCtrl(unsigned int n):bFull(this, &FIFOCtrl<T>::_f_check_full)
			,bEmpty(this, &FIFOCtrl<T>::_f_check_empty)
{
	n=(n==0)?1:n;
	_depth = n;
	_pool = new T[n];
	_head = 0;
	_tail = 0;
	//default threshold = depth / 2
	_threshold = _depth>>1;
}

template <class T>
FIFOCtrl<T>::~FIFOCtrl(void)
{
	delete _pool;
}

#endif //ndef _FIFO_CTRL_H_
