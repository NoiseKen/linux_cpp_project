#include "thread_locker.h"
//#include <unistd.h>
//---------------------------------------------------------------------------
void 
ThreadLocker::acquire(void)
{
    while(this->__locked)
    {
        //usleep(usec);
    }
    this->__locked=true;
};
//---------------------------------------------------------------------------
void
ThreadLocker::release(void)
{
    __locked=false;
}
//---------------------------------------------------------------------------
ThreadLocker::ThreadLocker(void)
{
    __locked=false;
}
//---------------------------------------------------------------------------
ThreadLocker::~ThreadLocker(void)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
