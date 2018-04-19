#ifndef _THREAD_LOCKER_H_
#define _THREAD_LOCKER_H_

/*
 * ThreadLockder : flag type locker
 *                 only support 1 thread activate in same time
 */
class ThreadLocker
{
public:
    void acquire(unsigned int usec=1000);
    void release(void);
    ThreadLocker(void);
    virtual ~ThreadLocker(void);
protected:
private:
    bool __locked;
};


/*
 * ThreadSemaphore : count type locker
 *                   user can give the amount of thread(s) activate in smae time
 */
class ThreadSemaphore
{
public:
    void acquire(unsigned int usec=1000);
    void release(void);
    ThreadSemaphore(unsigned int count=1);
    virtual ~ThreadSemaphore(void);
protected:
private:
    unsigned int __count;
};
#endif//ndef _THREAD_LOCKER_H_
