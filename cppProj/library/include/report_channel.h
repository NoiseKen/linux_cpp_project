#ifndef __REPORT_CHANNEL_H__
#define __REPORT_CHANNEL_H__
#include "thread_locker.h"

template<class T>
class ReportChannel
{
public:
	void operator>>(T &pop);
	void operator<<(T push);
	ReportChannel(void);
	~ReportChannel(void);
protected:
    ThreadLocker *_locker;
	T log;
private:
#define BIT_FLAG(b)		(1<<b)
	enum{
		logUpdating=0,	//for write
		logFetchDone,	//for read
	};
	unsigned int classFlag;
};

template<class T>
ReportChannel<T>::ReportChannel(void) // : report(this, &ReportChannel::_f_get_report, &ReportChannel::_f_set_report)
{
	classFlag = 0;
    _locker=new ThreadLocker();
	//log.clear();
    log="";
}

template<class T>
ReportChannel<T>::~ReportChannel(void)
{
    delete _locker;
}

template<class T>
void
ReportChannel<T>::operator>>(T &pop)
{
	T ret;
    _locker->acquire();
	if(classFlag & (BIT_FLAG(logUpdating)|BIT_FLAG(logFetchDone)))
	{
		//ret.clear();
        ret="";
	}
	else
	{		
		ret = log;
		classFlag |= BIT_FLAG(logFetchDone);		
	}
	
	pop = ret;
    _locker->release();
}

template<class T>
void
ReportChannel<T>::operator<<(T push)
{
    _locker->acquire();
	classFlag |= BIT_FLAG(logUpdating);
	if(classFlag & BIT_FLAG(logFetchDone))
	{		
		classFlag &= ~BIT_FLAG(logFetchDone);
		//log.clear();		
        log="";
	}	
	log += push;
    //?????????????????????
    //log += "\r\n";
	classFlag &= ~BIT_FLAG(logUpdating);
    _locker->release();
}

#endif //ndef __REPORT_CHANNEL_H__
