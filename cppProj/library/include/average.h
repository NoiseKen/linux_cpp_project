 //---------------------------------------------------------------------------
#ifndef __AVERAGE_H__
#define __AVERAGE_H__

//----------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <system.hpp>
#include "string.h"
#include "property.h"
//----------------------------------------------------------------------------
// class name: TAverage
// descriptor: Avarge the pushed type T or say the integration in type T
//
//     useage: call for easy calculate the average of type T
//
//  support operator: << push an type T item
//                    >> calculate the average of current item(s)
//    Example: TAverage<unsigned int> *ava = new TAverage<unsigned int>(3);	//new an average object in unsigned int maximum item count 3
//                  *ava << 12 << 13;	//push 12, 13
//                  *ava >> a;          //a will be (12+13)/2
//                  *ava << 14 << 15;	//push 14, 15, mean while first item (12) will be drop off
//                  *ava >> a;          //a will be (13+14+15)/3
//
//----------------------------------------------------------------------------
template <class T>
class Average
{
protected:
	unsigned int _f_get_item_count(void) {return itemCnt;};
private:
    T *record;
    unsigned int avgNum;
    unsigned int itemCnt;
public:
	property_ro<Average, unsigned int>count;
    class Average<T>& operator <<(T in);
    class Average<T>& operator >>(T& out);
    
    void clear(void);
   
    Average(unsigned int n);
    ~Average(void) { delete record; };
};

template <class T>
void Average<T>::clear(void)
{
	itemCnt = 0;
}

template <class T>
class Average<T>& Average<T>::operator <<(T in)
{

    if( itemCnt < avgNum )
    {
        record[itemCnt++] = in;
    }
    else
    {
        T *backUp;
        backUp = new T[avgNum];
        memcpy(backUp, &record[1], sizeof(T)*avgNum);
        //drop off 1st item, and backup all after
		delete record;
		record = backUp;
    }
    return *this;
}

template <class T>
class Average<T>& Average<T>::operator >>(T& out)
{
    int i;
    uint64_t average;
    average = (T)0;
    for(i=0;i<itemCnt;i++)
    {
        average += record[i];
    }

    out = (average==0)?0:average/itemCnt;
    return *this;
}

template <class T>
Average<T>::Average(unsigned int n) : count(this, &Average<T>::_f_get_item_count)
{
	itemCnt=0;
	avgNum=n; 
	record=new T[n];
}

//---------------------------------------------------------------------------
//extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
