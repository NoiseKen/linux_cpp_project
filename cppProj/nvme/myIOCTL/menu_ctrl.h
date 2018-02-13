#ifndef __MENU_CTRL_H__
#define __MENU_CTRL_H__

#include "menu_system.h"

//#define KEY_VALID	(1<<15)

//----- menuKeyCtrl->ctrlFlag ---------------
//#define KEY_IDENTFYING 				mBIT(15)
#define KEY_EXECTUE					mBIT(14)

#define TASK_RUNNING				mBIT(13)
#define TASK_WAIT_PARAMETER			mBIT(12)

#define TASK_CLOSE					mBIT(11)

#define HELP_VIEWING				mBIT(10)
#define WAIT_ANY_KEY				mBIT(9)

#define TASK_WAIT_ANSWER			mBIT(8)

//Task low frequency
#define	RELAXED_TASK				mBIT(7)		//

//#define INT_DATA_ABORT				mBIT(6)
//Task Running wait user key
#define TASK_PAUSE					mBIT(5)		//
#define GO_PAGE_PARAMETER			mBIT(4)

//Is Hint Showing
//#define HINT_SHOWING				mBIT(3)
//#define DUMP_ENDIATYPE				mBIT(2)
//#define DUMP_CONTINUE				mBIT(1)
#define LOG_MODE					mBIT(1)
#define PMARK_BLINKING				mBIT(0)

//#define STATUS_MASK					(COLORFUL_SHOW|DUMP_CONTINUE|DUMP_ENDIATYPE)

#define STATUS_MASK					(0)
//--------- menu key ctrl SS define ------------
//#define SPECIAL_KEY_IDENTFY_LOOP	0x1FF

#define MAX_MENU_LEVEL			8

#if (MAX_MENU_LEVEL&0x03)
#error MAX_MENU_LEVEL should be mutiple of 4
#endif

#define MAX_HISTORY_DEPTH		8

//------------- Screen define ------------------

//#define REPORT_LIMITED		32
#define REPORT_LIMITED		(ctrl->reportLimit)
//#define REPORT_ONE_PAGE		(REPORT_LIMITED*0x10)

#define REPORT_LINE(arg)	(TITLE_INFO_COUNT+1+(unsigned int)(arg))

	
#define HINT_LINE		REPORT_LINE(REPORT_LIMITED)
#define COMMAND_LINE	HINT_LINE+1

//------------- menu item ------------------
#define ITEM_ATTRIBUTE		0
#define ITEM_STRING 		1
#define ITEM_COUNT			2

//----- export public function ---------------
extern void menu_ctrl_init(TStrListSet mSet, MenuTaskSet tSet);
extern void menu_screen_redraw(void);
extern int menu_main_routine(void);
extern void menu_show_line(unsigned char line,TStr showstring);
#endif//def __MENU_CTRL_H__
