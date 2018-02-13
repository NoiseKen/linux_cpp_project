#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "timer_ctrl.h"

//status member need declare out side, not in class constructor
//here the link can determine the reference address for static member
unsigned int volatile TimerCtrl::TimerCore::msTick;
unsigned int volatile TimerCtrl::TimerCore::sTick;
unsigned int TimerCtrl::TimerCore::decSlot;				//decrement slot status
unsigned int TimerCtrl::TimerCore::decTick[TMR_SLOT_NUMBER];	//decrement slot tick count;
class TimerService TimerCtrl::TimerCore::services[TMR_SLOT_NUMBER];
bool TimerCtrl::TimerCore::bOSConfigured=false;			//bit flag OS already config timer		

class TimerCtrl::TimerCore TimerCtrl::core(APP_TIMER_US);

//void TimerCtrl::TimerCore::timer_core_handler(int);
TimerCtrl::TimerCore::TimerCore(int uSecond)
{
	msTick=0;
	sTick=0;
	decSlot = 0;		
	memset(decTick, 0, sizeof(decTick));
	memset(services, 0, sizeof(services));
    //configure linux timer
    struct itimerval t;
    int r;
	memset(&t, 0, sizeof(struct itimerval));
	
    //alarm SIG interval (2nd time after)    
    t.it_interval.tv_usec = uSecond;
    //t.it_interval.tv_sec = 0;
    //init alarm SIG event (1st time)
    t.it_value.tv_usec = uSecond;
    //t.it_value.tv_sec = 0;
	r = setitimer(ITIMER_REAL, &t, NULL);
	if(r==0)
	{	
		//configure handle to alarm SIG		
		signal( SIGALRM, &timer_core_handler); 
		bOSConfigured = true;
	}
}

TimerCtrl::TimerCore::~TimerCore(void)
{
	struct itimerval t;	
	memset(&t,0,sizeof(struct itimerval));
	setitimer(ITIMER_REAL, &t, NULL);
}

/*
 * function	: timer_handler
 * 			  1ms timer handler
 * parameter: [which]=should be TIME_REAL
 * return	: none
 */
void
TimerCtrl::TimerCore::timer_core_handler(int which)
{	
	bool bKeepCheck=true;
	int localSlot;
	unsigned int bit;
	
	localSlot = which;
	//global tick
	if(++msTick==1000)
	{
		msTick=0;
		sTick++;
	}
	
	localSlot = decSlot^-1;	//cpl slot bit

	while(bKeepCheck)
	{
		bit = search_free_slot_32((unsigned int)localSlot);
		if(bit == INVALID_ITEM)
		{
			bKeepCheck = false;
		}
		else
		{//found a running tick slot		
			if(decTick[bit]!=0)
			{				
				if(services[bit].status != SS_DEACTIVE)
				{
					decTick[bit]--;
				}
			}		
			
			if(services[bit].status==SS_ONE_SHOT)
			{
				if(decTick[bit]==0)
				{
					decSlot &= ~(1<<bit);	//remove bit from slot status
				}
			}				
					
			localSlot |= (1<<bit);
			
		}		
	}
}

void
TimerCtrl::TimerCore::core_routine(void)
{
	bool bKeepCheck=true;
	int localSlot;
	unsigned int bit;

	localSlot = decSlot^-1;	//cpl slot bit

	while(bKeepCheck)
	{
		bit = search_free_slot_32((unsigned int)localSlot);
		if(bit == INVALID_ITEM)
		{
			bKeepCheck = false;
		}
		else
		{//found a running tick slot		
			if(decTick[bit]==0)
			{				
				//check if service exist
				if(services[bit].fptrService!=NULL)
				{
					if(services[bit].status != SS_DEACTIVE)
					{
						services[bit].fptrService();
					}
				}
				
				if(services[bit].status !=SS_ONE_SHOT)
				{
					decTick[bit] = services[bit].interval;
				}
			}	
					
			localSlot |= (1<<bit);
			
		}		
	}
}


void
TimerCtrl::timer_slot_active(unsigned int id, bool bActive)
{	
	if(id<TMR_SLOT_NUMBER)
	{//valid slot number
		if((1<<id)&core.decSlot)
		{//slot in using
			if(core.services[id].status!=SS_ONE_SHOT)
			{//not one shot service
				core.services[id].status = (bActive)?SS_ACTIVE:SS_DEACTIVE;
			}
		}
	}	
}

class TimerCtrl& 
TimerCtrl::operator >> (TimeTick &tt)
{
	tt.ms = core.msTick;
	tt.s = core.sTick;
	return *this;
}

int 
TimerCtrl::operator <<(class TimerService in)
{
	unsigned int bit;
	if(in.status == SS_ONE_SHOT)
	{
		in.fptrService = NULL;
	}

	
	bit = search_free_slot_32((unsigned int)core.decSlot);
	if(bit != INVALID_ITEM)
	{
		memcpy(&core.services[bit], &in, sizeof(class TimerService));		
		core.decTick[bit] = in.interval;
		core.decSlot |= (1<<bit);
	}
	return bit;
}

unsigned int 
TimerCtrl::operator [](unsigned int i)
{
	return (i<TMR_SLOT_NUMBER)?core.decTick[i]:0;
}

bool
TimerCtrl::_f_check_experied(int i)
{
	//return (_decTick[i]==0)?true:false;
	return !(core.decSlot & (1<<i));
}

bool
TimerCtrl::_f_check_configured(void)
{
	return core.bOSConfigured;
}

void
TimerCtrl::routine(void)
{
	core.core_routine();
}
#if 1
void 
TimerCtrl::delay_ms(unsigned int ms)
{
	uint64_t target, now;	
	target = core.sTick*1000+core.msTick+ms;
//	printf("Target = %lu\n", target);
	do{
		routine();
		now = core.sTick*1000+core.msTick;
//printf("target=%lu, now = %lu\n", target, now);
	}while(now<target);
}
#endif
TimerCtrl::TimerCtrl() : bRunning(this, &TimerCtrl::_f_check_configured)
			,bExpired(this, &TimerCtrl::_f_check_experied)
{

}

TimerCtrl::~TimerCtrl(void)
{
}
