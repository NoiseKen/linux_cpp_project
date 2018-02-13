#ifndef _THREAD_LOCKER_H_
#define _THREAD_LOCKER_H_
class ThreadLocker
{
public:
    void acquire(void){while(locked){}; locked=true;};
    void release(void){locked=false;};
    ThreadLocker(void){locked=false;};
    ~ThreadLocker(void){};
protected:
private:
    bool locked;
};

#endif//ndef _THREAD_LOCKER_H_
