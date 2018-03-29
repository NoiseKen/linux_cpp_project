#ifndef _THREAD_LOCKER_H_
#define _THREAD_LOCKER_H_
class ThreadLocker
{
public:
    void acquire(unsigned int usec=1000);
    void release(void);
    ThreadLocker(void);
    ~ThreadLocker(void);
protected:
private:
    bool __locked;
};

#endif//ndef _THREAD_LOCKER_H_
