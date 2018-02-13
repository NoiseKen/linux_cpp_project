#ifndef __TIMER_CTRL_H__
#define __TIMER_CTRL_H__
#include "property.h"

#define APP_TIMER_US		1000
#define TMR_SLOT_NUMBER		(sizeof(int)*8)

typedef enum{
	SS_ONE_SHOT=0,
	SS_ACTIVE,
	SS_DEACTIVE,	
}TimeServiceStatus;

typedef class{
public:
	unsigned int ms;
	unsigned int s;
}TimeTick;

class TimerService
{
public:	
	//bool bRepeat;				//flag on repeat task, false for one shot
	TimeServiceStatus status;
	unsigned int interval;				//interval of millisecond
	void(*fptrService)(void);	//Timer Service point
};

/*
 * Class name : TimerCtrl
 * Description: linux timer controller HW
 * Usage :
 * 		class TimerService ts;
 *      TimerCtrl tmrCtrl;						//
 * 		if(tmrCtrl.bRunning)					//check timer controller construct success
 *		{ 
 * 			ts.status = SS_ONE_SHOT;			//an one shot service
 * 			ts.interval = 5;					//configure 5ms service	
 *			ts.fptrService = NULL;				//no service task be run
 * 			i = tmrCtrl << ts;					//push timer service & get which slot bit that timer controller used
 * 			while(!tmrCtrl.bExpired[i]);		//wait slot i expired
 * 			...
 *		}
 * 
 * 		while(1)
 * 		{
 * 			TimerCtrl::routine();				//put timer controller routine in main while 1 loop
 *		}
 * 		
 */
class TimerCtrl
{
private:			
	class TimerCore{
	public:
	static unsigned int volatile msTick;
	static unsigned int volatile sTick;
	static unsigned int decSlot;				//decrement slot status
	static unsigned int decTick[TMR_SLOT_NUMBER];	//decrement slot tick count;
	static class TimerService services[TMR_SLOT_NUMBER];
	static bool bOSConfigured;			//bit flag OS already config timer		
	static void timer_core_handler(int);
	static void core_routine(void);
	TimerCore(int);
	~TimerCore(void);
	};
	static TimerCore core;	
	bool _f_check_configured(void);
	bool _f_check_experied(int);
	//static void 
protected:
public:	
	//operator
	class TimerCtrl& operator >> (TimeTick &);
	//push operator
	int operator <<(class TimerService);	
	//pop operator, this operator get current slot tick count
	unsigned int operator [](unsigned int);
	//override event function (for inheritance)
	//method	
	void timer_slot_active(unsigned int, bool);
	static void routine(void);
	static void delay_ms(unsigned int ms);
	//useful properties	
	property_ro<TimerCtrl, bool> bRunning;
	properties_ro<TimerCtrl, bool> bExpired;	
	//constructor
	TimerCtrl(void);
	//destructor
	~TimerCtrl(void);	
};

#endif //ndef __TIMER_CTRL_H__
