#include <stdarg.h>
#include <string.h>
#include "menu_ctrl.h"
#include "key_ctrl.h"
#include "timer_ctrl.h"
#include "value_list.h"
#include "history_list.h"
#include "terminal_ctrl.h"
#include "legacy_string.h"
#include "utils.h"
#include "nvme_tool.h"

#define STATUS_SET(bit)	ctrl->flag |=  (bit)
#define STATUS_CLR(bit)	ctrl->flag &= ~(bit)
#define STATUS_MSK(bit)	ctrl->flag &= (bit)
#define STATUS_CPL(bit)	ctrl->flag ^= (bit)
#define STATUS_CMP(bit)	(ctrl->flag & (bit))

#define iCURRENT_IMENU      ctrl->pathList[ctrl->iLevel]

#define HINT_LINE_SHOW_MS		1500
#define PMARK_BLINKING_MS		500

typedef struct _MenuAttribute{
	//Next Menu Index
	unsigned char NI;
	//Item Attribture
	unsigned char IA;
	//Special Flag
	unsigned char SF;
	//keep in zero, not used
	unsigned char RES[sizeof(void*)-3];
} __attribute__((__packed__)) MenuAttribute;

typedef struct{
	TStr descriptor;
	unsigned char mode;
	unsigned char digital;	
	unsigned char reserved[2];
} __attribute__((__packed__)) ParaDescriptor;

typedef struct{	
	unsigned char iParameter;
	unsigned char cParameter;
	unsigned char reserved[2];
	unsigned char parBuffer[8];
} __attribute__((__packed__)) ParaTmpBuf;

typedef struct menu_ctrl_zi{
	unsigned char iActiveTask;	//active task index
	unsigned char iTaskPar;		//current parameter index
	unsigned char cTaskPar;		//total parameter count
	unsigned char iStrHistory;	//string history need be set zero whenever string parameter start
	//-----log related-----------
	//unsigned int logFlag;
	//-----parameter reated-------
	ParaTmpBuf	paraInput;
	ParaDescriptor paraInfo[MAX_PARAMETER_DEPTH];
	//-----export parameter
	MenuSystemExport out;	//parameter export to validation function
}MenuCtrlZI;

typedef struct menu_ctrl{
	//------flag-----------
	unsigned int flag;
	unsigned int relaxLoad;
	//------key ctrl--------
	unsigned char keyCode;
	unsigned char asciiCode;
	unsigned char reportLimit;
	unsigned char lastProgress;
	//------store basic msg---------
	TStrListSet	menuSet;
	MenuTaskSet taskSet;
	unsigned int historyList[MAX_HISTORY_DEPTH];
	unsigned char pathList[MAX_MENU_LEVEL];	//Menu path list / Buffer os iLevel
	unsigned char nodeList[MAX_MENU_LEVEL];	//Menu item list 
	//------menu ctrl-------
	unsigned char iLevel;		//Menu level index	
	unsigned char currentPos;	//Current Menu Position
	unsigned char iHistory;		//History Index
	unsigned char cHistory;		//History Count
	//-----ZI-----------
	MenuCtrlZI zi;
} __attribute__((__packed__)) MenuCtrl;

enum{
	SLOT_PMARK_BLINKING=0,
	SLOT_HINT_DISMISS,
	SLOT_RELAX_TASL_DEC,
};
static char activeSlot[4];

static MenuCtrl menuCtrl;

#define ctrl		(&menuCtrl)
#define exporter	(&ctrl->zi.out)

//-----------------------title------------------------------------------
static const TStr menuTitle[]=
{
	(TStr)"====================== APP Console ======================",
	(TStr)"*        PATH:",
	(TStr)"*   Task Name:",	
	(TStr)"*  Task Phase:",	
	(TStr)"* Parameter 0:",
	(TStr)"* Parameter 1:",
	(TStr)"* Parameter 2:",
	(TStr)"* Parameter 3:",
	//"==========================================================",
	(TStr)"======================================= Progressing :",
};

enum {	TITLE_LOCATE_LINE = 2,
		TITLE_TASK_LINE, TITLE_PHASE_LINE, //
		TITLE_PARAMETER_LINE,
		TITLE_PROGRESS_LINE=TITLE_PARAMETER_LINE+4,
		TITLE_INFO_COUNT = sizeof(menuTitle)/sizeof(TStr)
};

//--------------------------HELP INFO----------------------------------
static const TStr menu_help_info[]=
{
	(TStr)"F2         :ReDraw Screen",
	(TStr)"'Q' or 'q' :Exit application (only valid when menu status)",
	(TStr)"'C' or 'c' :direct command line input (only valid when menu status)",
	(TStr)"ESC        :Force Break QC Routine",	
	(TStr)"Build Date : ",
	(TStr)"Build Time : ",
	(TStr)__DATE__,
	(TStr)__TIME__,
};

enum {	helpF2 = 0,
		helpQ,
		helpC,
		helpESC,		
		helpBuildDate, helpBuildTime, 
		helpDateInfo, helpTimeInfo,
		helpInfoCount
};

//-------------------------Message Log----------------------------------

static const u8 PMark[]=
{
	' ','>',' ',__EOS
};

static const u8 SMark[]=
{
	' ',' ',' ',__EOS
};

enum {	logMemoryAddress = 0, logWriteData, 
		//general mark
		logEndOfString, logDataAbort, logPointMark, logSpaceMark,
		//format
		logDumpFormat,log8DigHexFormat, logGoFunctionFormat,
		//Hint String
		logRootMenu, logPhaseWaitParameter, logPhaseRunning, logKeyNotAllow, logNotifyHint,
		logEnterPageNumber, logEndLessTaskHint, logHelpViewHint, 
		logForceBreakTask, logTaskNotAssign, logUndefineResult, logAnyKeyContinue,
		//Task Result
		logTestPass, logTestFail,
		logInvalidParameter, logTimeOutError, logCompareError,
		//Task Phase
		logPhaseDone, logPhaseCancel, logPhaseCrash, logPhaseTaskNotExist,
		//FW info
		logDate, logTime
		
};

static const TStr menuMsgLog[]=
{		
	(TStr)"Memory Address:0x",
	(TStr)"Write Data:0x",
	//general mark
	(TStr)"",
	(TStr)" !!!!-!!!!",
	(TStr)PMark,
	(TStr)SMark,
	//format
	(TStr)" %04X-%04X",
	(TStr)"0x%08X:",
	(TStr)"---ESC:EXIT, G:Direct Go, %c:Page Up, %c:Page Down, --- page%d ---",
	//Hint String
	(TStr)"ROOT MENU",
	(TStr)"TASK WAIT PARAMETER!!",
	(TStr)"TASK RUNNING!!",
	(TStr)"KEY_NOT_ALLOW!!",
	(TStr)"Please press 'h' to enter help page",
	(TStr)"Please Enter The Page Number:",
	(TStr)"Please Press 'ESC' to cancel End-Less Task!!",
	(TStr)"Please Press 'ESC' to back Menu!",
	(TStr)"Force Break QC Task OK!!",
	(TStr)"Not Assign Task!!",
	(TStr)"Un-Defined Result!!",
	(TStr)"---Please Press Any Key to continue!!---",
	//Task Result
	(TStr)"TEST PASS!!",							//0
	(TStr)"TEST FAIL!!",							//1
	(TStr)"INVALID PARAMETER!!",					//2
	(TStr)"Task Time Out Error (No Response)!!",	//3
	(TStr)"Read/Write Compare Error!!",			//4		
	//Task Phase
	(TStr)"TASK DONE!!",		//0
	(TStr)"TASK CANCEL!!",	//1
	(TStr)"TASK CRASH!!",		//2
	(TStr)"TASK NOT EXIST!!",	//3
};

static void
set_number(unsigned char num)
{
	TerminalCtrl term;
	unsigned char c;	
	if( num/100 )
	{
		c = '0'+(num)/100;
	}
	else
	{
		c = ' ';
	}
		
	term << c;
	
	num%=100;		
	c = '0'+(num)/10;	
	term << c;
	
	num%=10;
	term << '0'+num << '.';
}


/**
 *  memory dump
 *	@Attribution private
 *  @param[in]   pmem : target memory
 *  @param[in]   showaddress : start address display on screen
 *  @param[in]   dumpline : how many line (16bytes) want to view
 *  @return      none
 *
 */
static void 
show_dump(unsigned int* pmem, unsigned int showaddress, unsigned int dumpline)
{
	TerminalCtrl term;
	//unsigned int linkFlag;	
	unsigned int loop;
	unsigned char count;
	char *tmpBuf;
	//char message[20];	
	
	tmpBuf = new char[64];
		
	//if(dumpline > REPORT_LIMITED)
	//{
	//	dumpline = REPORT_LIMITED;
	//}
	
	pmem = (unsigned int *)((uint64_t)pmem & ~0x03);	//fix to u32 mode
	
	//menu_clr_report(0);	//clr report & force lr to STACK
	
	for(loop = 0;loop < dumpline;loop++)
	{		
		if(loop & 0x01)
		{			
			term << COLOR_SEPERATE;
		}
		else
		{		
			term << COLOR_NORMAL;
		}
		
		//Head		
		printf(menuMsgLog[log8DigHexFormat],showaddress);	
		showaddress += 0x10;		
		count = 4;
		do
		{
			unsigned char *c;
			c = (unsigned char *)pmem;									
			printf(" %02X%02X-%02X%02X",c[0],c[1],c[2],c[3]);
			pmem++;
		}while(--count);
		printf("\n");
		
	}

	delete tmpBuf;

	term << COLOR_NORMAL;
}

/**
 *  memory dump function
 *	@Attribution public
 *  @param[in]   mode : operate mode
 *  @return      none
 *
 */
void
memory_dump(uint64_t raddr, unsigned int nLBA)
{
	unsigned int row;
	raddr &= ~0x03;
	if(nLBA!=0)
	{
		row = nLBA*(512/16);	
		show_dump((unsigned int*)raddr,0,row);
	}
}


static void menu_blinking_selected(void);
static void menu_hint_line_dismiss(void);
/**
 *  menu control module init
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void
menu_ctrl_init(TStrListSet mSet, MenuTaskSet tSet)
{	
	memset( ctrl, 0, sizeof(MenuCtrl) );
	ctrl->menuSet = mSet;
	ctrl->taskSet = tSet;

	activeSlot[SLOT_PMARK_BLINKING] = TimerCtrl() << (TimerService){ SS_ACTIVE, PMARK_BLINKING_MS, menu_blinking_selected };
	activeSlot[SLOT_HINT_DISMISS] = TimerCtrl() << (TimerService){ SS_DEACTIVE, HINT_LINE_SHOW_MS, menu_hint_line_dismiss };
}

/**
 *  show string in assigned line
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void 
menu_show_line(unsigned char line,TStr showstring)
{		
	TerminalCtrl term;
//	if(IntegrationF & REPORT_SHOW_HOLD)
//	return;
	//if(0 == STATUS_CMP(LOG_MODE)
	//||(line == COMMAND_LINE))
	//{
		//ansi_gotoxy(0,line);
		//mClr_Line;
		term << (XYLocate){0, line} << AnsiGen::EC_CLR_AFTER;
		
		if(showstring!=NULL)
		{
			//ASCII_OUT(showstring);
			term << showstring;
			if(line == HINT_LINE)
			{
				TimerCtrl().timer_slot_active(activeSlot[SLOT_HINT_DISMISS], true);				
			}	
		}
	//}
}

static void 
menu_title_update( unsigned int iTitle, TStr update )
{
	if(0 == STATUS_CMP(LOG_MODE))
	{		 
		XYLocate l;
		l.x = strlen((const char*)menuTitle[iTitle-1])+2;
		l.y = iTitle;
		TerminalCtrl() << AnsiGen::EC_SAVE_CURSOR << l << AnsiGen::EC_CLR_AFTER << update << AnsiGen::EC_RESTORE_CURSOR;
	}
}

static void
menu_task_run_init(void)
{
	TerminalCtrl term;
	STATUS_SET(TASK_RUNNING);
	menu_title_update(TITLE_PHASE_LINE,menuMsgLog[logPhaseRunning]);
	menu_clr_report(0);	
	term << (XYLocate){ 1, REPORT_LINE(0) };
	term.flush();

}

/**
 *  show string in report line
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void 
menu_show_report(unsigned char index ,TStr showstring)
{		
	menu_show_line( REPORT_LINE(index), showstring );
}


/**
 *  show string in hint line
 *	@Attribution public
 *  @param[in]   horn : HINT or NOTICE, show color
 *  @return      none
 *
 */
void 
menu_show_hint(unsigned char horn ,TStr showstring)
{
	TerminalCtrl term;
	if(0==STATUS_CMP(LOG_MODE))		
	{
		if( horn == NOTICE )
		{
			term << COLOR_NOTICE;
		}
		else
		{
			term << COLOR_FOCUS;
		}

		menu_show_line(HINT_LINE,(TStr)showstring);
		term << COLOR_NORMAL;
	}
}

/**
 *  clr report region
 *	@Attribution public
 *  @param[in]   ireport : start index, 0 : whole report
 *  @return      none
 *
 */
void 
menu_clr_report( unsigned char ireport )
{	
	TerminalCtrl term;
	ireport %= REPORT_LIMITED;
	term << (XYLocate){1, REPORT_LINE(ireport)};
	do
	{
		term << AnsiGen::EC_CLR_AFTER << AnsiGen::EC_CURSOR_DOWN;
	}while(REPORT_LINE(++ireport) != HINT_LINE);
	//ansi_gotoxy(1,REPORT_LINE(0));
}

/**
 *  get menu information
 *	@Attribution private
 *  @param[in]   imenu : index of menu
 *  @param[in]   iitem : index of item
 *  @param[in]   mitem : mode of item ITEM_STRING or ITEM_ATTRIBUTE or ITEM_COUNT
 *  @return      menu item string
 *
 */
static TPurePoint 
GetMenuItem(unsigned char imenu, unsigned char iitem, unsigned char mitem)
{
	TStrList pmlist;
	unsigned int offset;
	unsigned long int result;
	
	pmlist = ctrl->menuSet[imenu];	
	
	switch(mitem)
	{
		case ITEM_STRING:
		case ITEM_ATTRIBUTE:
			offset = (iitem<<1)+mitem;
			result = (unsigned long int)pmlist[offset];
			break;
		case ITEM_COUNT:
			result = 0;
			while(*pmlist != __EOM)
			{
				result++;
				pmlist++;
			}
			result >>= 1;
			break;
		default:
			result = INVALID_ITEM;
	}
	return (TPurePoint)result;
}

void
ChangeItemAttribute(TStrList ptrMenuList, unsigned int idxItem, unsigned char attr)
{
	MenuAttribute *CA;
	CA = (MenuAttribute *)&ptrMenuList[(idxItem<<1)];
	CA->IA = attr;
}

static void
plot_menu_item(unsigned char imenu, unsigned char iitem)
{
	TerminalCtrl term;
	MenuAttribute CA;
	*(unsigned long int*)&CA = (unsigned long int)GetMenuItem(imenu,iitem,ITEM_ATTRIBUTE);
	switch(CA.IA)
	{
		case ATTR_TASK:
			term << COLOR_SYMBOL_TASK;
			break;
		case ATTR_FOLDER:
			term << COLOR_SYMBOL_FOLDER;
			break;
		case ATTR_STRING:
			term << COLOR_SYMBOL_STRING;
			break;
	}
		
	term << (TStr)GetMenuItem(imenu,iitem,ITEM_STRING);
	
}

/**
 *  Event when menu move up or down
 *	@Attribution private
 *  @param[in]   direction : Up, Down, Number Key
 *  @return      none
 *
 */
static void 
MenuMove(unsigned char direction)
{
	unsigned char imenu;	
	unsigned char max_pos;
	unsigned char current_pos;
	unsigned char update_pos;
	TerminalCtrl term;

	imenu = iCURRENT_IMENU;
	
	max_pos = (unsigned long int)GetMenuItem(imenu,0,ITEM_COUNT) - 1;
	current_pos = ctrl->currentPos;
	update_pos = 0;
	//Update Current Select	
	if( direction == KEY_UP )
	{
		if(current_pos==0)
		{
			update_pos = max_pos;
		}
		else
		{
			update_pos = current_pos-1;
		}

	}
	else
	if( direction == KEY_DOWN )
	{
		if(current_pos==max_pos)
		{
			update_pos = 0;
		}
		else
		{
			update_pos = current_pos+1;
		}
	}
	else
	if(direction == KEY_HOME)
	{
		update_pos = 0;
	}
	else
	if(direction == KEY_END)
	{
		update_pos = max_pos;
	}
	else
	if(direction == KEY_PAGE_UP)
	{
		if( current_pos < REPORT_LIMITED )
		{
			update_pos = 0;
		}
		else
		{
			update_pos = current_pos - REPORT_LIMITED;			
		}
	}
	else
	if(direction == KEY_PAGE_DOWN)
	{				
		if( (max_pos - current_pos) < REPORT_LIMITED )
		{
			update_pos = max_pos;			
		}
		else
		{
			update_pos = current_pos + REPORT_LIMITED;			
		}
	}

	if( current_pos != update_pos )
	{
		ctrl->currentPos = update_pos;

		if( (update_pos/REPORT_LIMITED) == (current_pos/REPORT_LIMITED) )
		{//@ smae display page, manul			
			term << COLOR_NORMAL;
			menu_show_line(REPORT_LINE(current_pos % REPORT_LIMITED),menuMsgLog[logSpaceMark]);	
			set_number(current_pos);			
			plot_menu_item(imenu,current_pos);
			term << COLOR_NORMAL << COLOR_SELECTED;
			menu_show_line(REPORT_LINE(update_pos % REPORT_LIMITED),menuMsgLog[logPointMark]);
			set_number(update_pos);
			plot_menu_item(imenu,update_pos);
			term << COLOR_NORMAL;					
		}
		else
		{
			menu_screen_redraw();
		}
	}
	term << (XYLocate){0, COMMAND_LINE};
}


/**
 *  Event when menu move left or right
 *	@Attribution private
 *  @param[in]   direction : Left or right key
 *  @return      none
 *
 */
static void 
MenuChange(unsigned char direction)
{
#define RE_DRAW		mBIT(0)
#define ILL_SEL		mBIT(1)

	unsigned char imenu;
	unsigned char flag;		
	MenuAttribute CA;
	
	flag = 0;
	
	imenu = iCURRENT_IMENU;
	
	switch(direction)
	{
		case KEY_LEFT:
			if(imenu != MENU_ROOT)
			{
				ctrl->iLevel--;
				ctrl->currentPos = ctrl->nodeList[ctrl->iLevel];
				flag |= RE_DRAW;
			}
			break;
		case KEY_RIGHT:
		case KEY_ENTER:
			*(unsigned int*)&CA = (unsigned long int)GetMenuItem(imenu,ctrl->currentPos,ITEM_ATTRIBUTE);
			
			if(CA.IA == ATTR_FOLDER)		
			{//if Current Attribute is Folder				
				ctrl->nodeList[ctrl->iLevel] = ctrl->currentPos; 
				ctrl->iLevel++;	
				iCURRENT_IMENU = CA.NI;				
				ctrl->currentPos = 0;
				flag |= RE_DRAW;
			}
			else							
			if(CA.IA == ATTR_TASK) 
			{//Is Task Selected
				TStr title;								
				
				title = (TStr)GetMenuItem(iCURRENT_IMENU,ctrl->currentPos,ITEM_STRING);
				
				ctrl->zi.iActiveTask = CA.NI; 
				//move to "after init function
				//menu_title_update(TITLE_TASK_LINE,title);
				
				if(CA.NI > TASK_MAX_COUNT)
				{
					//not assign this task
					flag |= ILL_SEL;	
				}
				else
				{			
					//update special flag
					exporter->testOP = CA.SF;
					
					//running a task deactive blinking point mark					
					TimerCtrl().timer_slot_active(activeSlot[SLOT_PMARK_BLINKING], false);
					
					//execute init function, please push question in init function
					if(ctrl->taskSet[CA.NI].init != NULL)
					{
						ctrl->taskSet[CA.NI].init(CA.SF);
					}
				}				
				
				if(flag & ILL_SEL)
				{					
					STATUS_SET(TASK_RUNNING);
				}
				else
				{
					if( ctrl->zi.cTaskPar != 0 )
					{
						STATUS_SET(TASK_WAIT_PARAMETER);
						menu_title_update(TITLE_PHASE_LINE,menuMsgLog[logPhaseWaitParameter]);
						menu_show_line(COMMAND_LINE, ctrl->zi.paraInfo[0].descriptor);
					}
					else
					{
						menu_task_run_init();
					}					
				}
				
				menu_title_update(TITLE_TASK_LINE,title);				
			}
			break;			
	}
		
	if(flag & RE_DRAW)
	{
		menu_screen_redraw();
	}
	
#undef RE_DRAW	//	mBITX(0)
#undef ILL_SEL	//	mBITX(1)	
}

/**
 *  draw title
 *	@Attribution private
 *  @param[in]   none
 *  @return      none
 *
 */
static void 
menu_draw_title(void)
{
	unsigned char loop;
//	unsigned char ilastmenu;
	unsigned char ilastpos;
	TerminalCtrl term;
	
	ilastpos = ctrl->iLevel-1;
//	ilastmenu = ctrl->pathList[ilastpos];
	ilastpos = ctrl->nodeList[ilastpos];

	if(0==STATUS_CMP(LOG_MODE))	
	{
		for(loop = 0;loop < TITLE_INFO_COUNT;loop++)
		{				
			
			menu_show_line(loop+1,menuTitle[loop]);

			if(loop == 1)
			{			
#if 0			
				if(ctrl->iLevel==0)
				{
					ASCII_OUT(menuMsgLog[logRootMenu]);
				}
				else
				{
					ASCII_OUT((TStr)GetMenuItem(ilastmenu,ilastpos,ITEM_STRING));
				}
#else
				unsigned int i;			
				term << menuMsgLog[logRootMenu];
				for(i=0;i<ctrl->iLevel;i++)
				{
					term << '\\' << ' ' << (TStr)GetMenuItem(ctrl->pathList[i],ctrl->nodeList[i],ITEM_STRING);
				}
#endif	
			}
		}
	}
}
		
/**
 *  redraw screen
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void
menu_screen_redraw(void)
{
	TStr pmark;
	unsigned char loop;	
	unsigned char imenu;
	unsigned char iStart;
	unsigned char iEnd;
	TerminalCtrl term;
	struct winsize w;
	
	memset(&w, 0, sizeof(struct winsize));
	if( 0 == TerminalCtrl::get_terminal_resolution(&w) )
	{
		ctrl->reportLimit = w.ws_row - TITLE_INFO_COUNT - 2;
	}
	else
	{
		ctrl->reportLimit = 32;
	}

	term << COLOR_NORMAL << AnsiGen::EC_CLR_SCREEN;

	menu_draw_title();

	imenu = iCURRENT_IMENU;

	//get the page of current position 	
	iStart = (ctrl->currentPos/REPORT_LIMITED) * REPORT_LIMITED;	

	iEnd = (unsigned long int)GetMenuItem(imenu,0,ITEM_COUNT);

	if( (iEnd - iStart)>REPORT_LIMITED )
	{
		iEnd = iStart + REPORT_LIMITED;
	}
			
	for(loop = iStart;loop < iEnd;loop++)
	{				
		if(ctrl->currentPos == loop)
		{			
			term << COLOR_SELECTED;
			pmark = menuMsgLog[logPointMark];
		}		
		else
		{
			pmark = menuMsgLog[logSpaceMark];
		}
		
		menu_show_line(REPORT_LINE(loop-iStart),pmark);

		set_number(loop);
		
		//ASCII_OUT((TStr)GetMenuItem(imenu,loop,ITEM_STRING));
		plot_menu_item(imenu, loop);
		term << "\n\r" << COLOR_NORMAL;
	}
	
	term << (XYLocate){0, COMMAND_LINE};

}

static void
menu_blinking_selected(void)
{
	unsigned char row;
#if 1	
	TerminalCtrl term;	
	row = REPORT_LINE(ctrl->currentPos%REPORT_LIMITED);	

	term << (XYLocate){2, row} << COLOR_SELECTED;
	
	if(STATUS_CMP(PMARK_BLINKING))
	{
		term << PMark[1];
	}
	else
	{
		term << SMark[1];
	}
	
	STATUS_CPL(PMARK_BLINKING);
	
	term << COLOR_NORMAL << (XYLocate){0, COMMAND_LINE};
#endif	
}

static void 
menu_hint_line_dismiss(void)
{	
	menu_show_line(HINT_LINE,menuMsgLog[logEndOfString]);
	TimerCtrl().timer_slot_active(activeSlot[SLOT_HINT_DISMISS], false);
}

/**
 *  show help screen
 *	@Attribution private
 *  @param[in]   none
 *  @return      none
 *
 */
static void 
menu_show_help(void)
{
	unsigned char i;	
	char *msg;
	TerminalCtrl term;
	TimerCtrl().timer_slot_active(activeSlot[SLOT_PMARK_BLINKING], false);
	
	i=0;
	term << AnsiGen::EC_CLR_SCREEN;
	//Draw Title
	menu_draw_title();
	
	term << COLOR_HI_GREEN;
	menu_show_line(REPORT_LINE(i++),(TStr)"-------HOT KEY GROUP------");	
	term << COLOR_NORMAL;
	//F1. ReDraw Screen
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpF2]);
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpQ]);
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpC]);
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpESC]);
	i++;i++;
	term << COLOR_HI_GREEN;
	menu_show_line(REPORT_LINE(i++),(TStr)"-------MENU & TASK OPERATION------");
	term << COLOR_NORMAL;
	menu_show_line(REPORT_LINE(i++),(TStr)"ARROW UP/ ARROW DOWN :select menu/task item");
	menu_show_line(REPORT_LINE(i++),(TStr)"ARROW LEFT           :back to parent menu");
	menu_show_line(REPORT_LINE(i++),(TStr)"ARROW RIGHT/ ENTER   :enter menu/ run task item");
	i++;i++;

	term << COLOR_HI_GREEN;
	menu_show_line(REPORT_LINE(i++),(TStr)"-------MENU Item Symbol------");
	term << COLOR_SYMBOL_FOLDER;
	menu_show_line(REPORT_LINE(i++),(TStr)"White :Folder item, a sub folder");
	term << COLOR_SYMBOL_TASK;
	menu_show_line(REPORT_LINE(i++),(TStr)"Yellow :Task item, executable function");	
	term << COLOR_SYMBOL_STRING;
	menu_show_line(REPORT_LINE(i++),(TStr)"Gray :String item, cannot do any operation");
	i++;i++;


	term << COLOR_HI_GREEN;
	menu_show_line(REPORT_LINE(i++),(TStr)"-------APPLICATION INFORMATION------");
	term << COLOR_NORMAL;
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpBuildDate]);	
	term << menu_help_info[helpDateInfo];
	menu_show_line(REPORT_LINE(i++),(TStr)menu_help_info[helpBuildTime]);
	term << menu_help_info[helpTimeInfo];
	i++;i++;
	
	msg = new char[64];
	
	sprintf(msg, "Support driver version: %02d.%02d.%04d", 
								(IOCTL_APP_VER>>24)&0xFF,
								(IOCTL_APP_VER>>16)&0xFF,
								(IOCTL_APP_VER>>0)&0xFFFF
								);
	term << COLOR_HI_RED;
	menu_show_line(REPORT_LINE(i++), msg);
	term << COLOR_NORMAL;
	delete msg;	
}

/**
 *  sub event when key on execute menu ss in normal state
 *	@Attribution Private
 *  @param[in]   none
 *  @return      none
 *
 */
static void 
memu_key_when_menu(void)
{
	switch(ctrl->keyCode)	
	{
		case KEY_UP:
		case KEY_DOWN:		
		case KEY_HOME:
		case KEY_END:
		case KEY_PAGE_UP:
		case KEY_PAGE_DOWN:
			MenuMove(ctrl->keyCode);
			break;
		case KEY_LEFT:
		case KEY_RIGHT:
		case KEY_ENTER:
			MenuChange(ctrl->keyCode);
			break;
		//case KEY_ESC:
		//	break;
		case KEY_F2:
		//	STATUS_CPL(COLORFUL_SHOW);
		//case KEY_F1:
			menu_screen_redraw();
			break;

		case KEY_C:
			menu_direct_task(TASK_SYSTEM_BASIC, OP_COMMAND_LINE_TEST);
			break;
		case KEY_H:						
			STATUS_SET(HELP_VIEWING);
			menu_show_help();			
			break;					
		case KEY_NOT_ALLOW:
		default:
			menu_show_hint(HINT,menuMsgLog[logNotifyHint]);
			break;			
	}	
}

static void
menu_key_when_string_parameter(void)
{
#define UPDATE_PAR		mBIT(0)
#define MAKE_DONE		mBIT(2)	
	ParaDescriptor *desp;	
	ParaTmpBuf *para;
	unsigned char *ptr;
	HistoryList *history;
	unsigned int cntHistory;
	char flag;
	
	
	unsigned char loop;
	flag = 0;
	desp = &ctrl->zi.paraInfo[ctrl->zi.iTaskPar];	//string parameter only support in descriptor 0
	para = &ctrl->zi.paraInput;
	ptr = &exporter->arguments[para->iParameter];	//current cursor point
	//get current history count
	cntHistory = LegacyString::to_integer(ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_STR_HIS_DEPTH))); 
	history = new HistoryList(cntHistory);
	
	history->load_from(APP_HISTORY_FILE_PATH);
	cntHistory = history->size();
	
	switch(ctrl->keyCode)
	{
		case KEY_BACK:
			if(para->iParameter != 0)
			{
				//start from current index end at last digital
				for(loop=para->iParameter;loop<para->cParameter;loop++)
				{
					exporter->arguments[loop-1] = exporter->arguments[loop];
				}
									
				para->iParameter--;				
				para->cParameter--;
			}
			
			flag |= UPDATE_PAR;
			break;
		case KEY_DEL:
			if(para->cParameter > para->iParameter)
			{
				//start from current index end at last digital
				for(loop=para->iParameter+1;loop<para->cParameter;loop++)
				{
					exporter->arguments[loop-1] = exporter->arguments[loop];
				}
					
				para->cParameter--;
			}
			
			flag |= UPDATE_PAR;
			break;						
		case KEY_LEFT:
			if(para->iParameter!=0)
			{
				para->iParameter--;
			}
			break;
		case KEY_RIGHT:
			if(para->iParameter!=para->cParameter)
			{
				para->iParameter++;
			}
			break;
		case KEY_HOME:
			para->iParameter = 0;
			break;
		case KEY_END:
			para->iParameter = para->cParameter;
			break;	
		// Get String History
		case KEY_UP:
			if(ctrl->zi.iStrHistory>0)
			{
				ctrl->zi.iStrHistory--;
			}
			else
			{
				ctrl->zi.iStrHistory = cntHistory-1; 
			}		
			goto extractHis;
		case KEY_DOWN:
			if(ctrl->zi.iStrHistory<(cntHistory-1))
			{
				ctrl->zi.iStrHistory++;
			}
			else
			{
				ctrl->zi.iStrHistory = 0;
			}
extractHis:
			//get_txt_file_row(LOG_HISTORY_FILE_PATH, (char *)exporter->arguments, ARGUMENT_DEPTH, ctrl->zi.iStrHistory);
			{
				std::string extract;
				extract = (*history)[ctrl->zi.iStrHistory];
				memcpy(exporter->arguments, extract.c_str(), extract.size());
				para->iParameter=para->cParameter=extract.size();
				flag |= UPDATE_PAR;
			}
			break;
		case KEY_ENTER:
			flag |= MAKE_DONE;
			break;
		case KEY_ESC:
			ctrl->keyCode = KEY_CANCEL;
			STATUS_SET(TASK_CLOSE);			
		default:
			if(ctrl->asciiCode != 0xFF)
			{//normal text ascii code
				if(para->cParameter!=desp->digital)
				{			
					//start from last digital end at current index
					for(loop=para->cParameter;loop>para->iParameter;loop--)
					{
						exporter->arguments[loop] = exporter->arguments[loop-1];
					}
				
					*ptr = ctrl->asciiCode;

					para->cParameter++;				
					para->iParameter++;									
				
					flag |= UPDATE_PAR;
				}
			}
			else
			{
				return;
			}									
			break;					
	}
	
	TerminalCtrl term;
	
	if(flag & UPDATE_PAR)
	{
		exporter->arguments[para->cParameter]=__EOS;
		menu_show_line(COMMAND_LINE,desp->descriptor);
		term << (char *)exporter->arguments;
	}		

	loop = strlen((const char *)desp->descriptor) + 1;
	term << (XYLocate){(unsigned int)loop+para->iParameter, COMMAND_LINE};
	
	if(flag & MAKE_DONE)
	{

		if(STATUS_CMP(TASK_WAIT_ANSWER))
		{//interactive question in test function
			STATUS_CLR(TASK_WAIT_ANSWER);
			ctrl->zi.iTaskPar++;
		}
		else
		{//init function question
			//string should only support 1 parameter
			ctrl->zi.iTaskPar++;
			//if(ctrl->zi.cTaskPar == ++ctrl->zi.iTaskPar )
			{
				STATUS_CLR(TASK_WAIT_PARAMETER);
				
			}
		}		
		menu_task_run_init();
		//update history						
		*history << (char *)exporter->arguments;
		history->save_to(APP_HISTORY_FILE_PATH);
		//MEMORY_SLOT_RELEASE(buf);
		if(STATUS_CMP(TASK_WAIT_PARAMETER))
		{			
			desp++;// = &ctrl->paraInfo[ctrl->iTaskPar];
			menu_show_line(COMMAND_LINE,desp->descriptor);
		}
		
		memset( para, 0, sizeof(ParaTmpBuf) );				
	}
	
	delete history;	
#undef UPDATE_PAR	//	mBIT(0)
#undef MAKE_DONE	//	mBIT(2)	
}

/**
 *  event on menu key execute when parameter phase
 *	@Attribution Private
 *  @param[in]   none
 *  @return      none
 *
 */
static void 
menu_key_when_parameter(void)
{
#define UPDATE_PAR		mBIT(0)
#define MAKE_DONE		mBIT(2)
#define PARAMETER_SELECT	mBIT(3)
	unsigned char flag;
	unsigned char loop;
	unsigned char digitallimit;
	unsigned int parvalue;
	ParaDescriptor *desp;
	ParaTmpBuf *para;	
		
	parvalue = ctrl->keyCode;

	desp = &ctrl->zi.paraInfo[ctrl->zi.iTaskPar];

	para = &ctrl->zi.paraInput;

	flag = 0;	
		
	switch(desp->mode)
	{
		case modeDEC:
			flag |= PARAMETER_SELECT;	
			break;			
		case modeSTR:
			menu_key_when_string_parameter();
			return;
		
	}
	
	digitallimit = desp->digital;

	switch(parvalue)
	{
		case KEY_A:
		case KEY_B:
		case KEY_C:
		case KEY_D:
		case KEY_E:
		case KEY_F:
			if(flag & PARAMETER_SELECT)		
			return;
			
			parvalue = (parvalue -KEY_A) + KEY_0 + 10;
		case KEY_0:
		case KEY_1:				
		case KEY_2:			
		case KEY_3:
		case KEY_4:				
		case KEY_5:			
		case KEY_6:
		case KEY_7:				
		case KEY_8:			
		case KEY_9:
			parvalue = (parvalue - KEY_0);
			if(para->cParameter!=digitallimit)
			{			
				//start from last digital end at current index
				for(loop=para->cParameter;loop>para->iParameter;loop--)
				{
					para->parBuffer[loop] = para->parBuffer[loop-1];
				}
				
				para->parBuffer[para->iParameter] = parvalue;

				para->cParameter++;				
				para->iParameter++;									
				
				flag |= UPDATE_PAR;
			}												
			break;
		case KEY_BACK:
			if(para->iParameter != 0)
			{
				//start from current index end at last digital
				for(loop=para->iParameter;loop<para->cParameter;loop++)
				para->parBuffer[loop-1] = para->parBuffer[loop];				
				
				para->iParameter--;				
				//if(ParCount!=0)	
				para->cParameter--;
			}
			
			flag |= UPDATE_PAR;
			break;
		case KEY_DEL:
			if(para->cParameter > para->iParameter)
			{
				//start from current index end at last digital
				for(loop=para->iParameter+1;loop<para->cParameter;loop++)
				para->parBuffer[loop-1] = para->parBuffer[loop];				
	
				para->cParameter--;
			}
			
			flag |= UPDATE_PAR;
			break;			
			
		case KEY_LEFT:
			if(para->iParameter!=0)
			{
				para->iParameter--;
			}
			break;
		case KEY_RIGHT:
			if(para->iParameter!=para->cParameter)
			{
				para->iParameter++;
			}
			break;
		case KEY_HOME:
			para->iParameter = 0;
			break;
		case KEY_END:
			para->iParameter = para->cParameter;
			break;	
		// Get History
		case KEY_UP:
		case KEY_DOWN:
			if(flag & PARAMETER_SELECT)
			{
				if(parvalue == KEY_UP)
				{					
					if(++para->parBuffer[para->iParameter]==10)
					{
						para->parBuffer[para->iParameter]=0;
					}
				}
				else
				{
					if(--para->parBuffer[para->iParameter]==0xFF)
					{
						para->parBuffer[para->iParameter]=9;
					}
				}				
			}
			else
			{
				if(parvalue == KEY_UP)
				{
					if(ctrl->iHistory>0)
					{
						ctrl->iHistory--;
					}
					else
					{
						ctrl->iHistory = MAX_HISTORY_DEPTH-1; 
					}
				}
				else
				{
					if(ctrl->iHistory<(MAX_HISTORY_DEPTH-1))
					{
						ctrl->iHistory++;
					}
					else
					{
						ctrl->iHistory = 0;
					}
				}
				
				para->cParameter = digitallimit;
				parvalue = ctrl->historyList[ctrl->iHistory];			
				for(loop = 0;loop<digitallimit;loop++)
				{
					unsigned int t32;
					t32 = (digitallimit-1) << 2;
					t32 -= loop << 2;					
					para->parBuffer[loop] = (parvalue>>t32)&0x0F;
				}											
			}
			flag |= UPDATE_PAR;
			break;
		case KEY_ENTER:						
			flag |= MAKE_DONE;
			break;
		case KEY_ESC:
			ctrl->keyCode = KEY_CANCEL;
			STATUS_SET(TASK_CLOSE);			
		default:
			return;
	}
	
	TerminalCtrl term;
	
	if(flag & UPDATE_PAR)
	{
		unsigned char *showBuf;
		showBuf = new unsigned char[64];
		for(loop=0;loop<para->cParameter;loop++)
		{
			parvalue = para->parBuffer[loop];
			showBuf[loop] = '0' + parvalue;
			if(parvalue > 9)
			{
				showBuf[loop] += 7;
			}
		}
		showBuf[loop] = __EOS;
		menu_show_line(COMMAND_LINE,desp->descriptor);				
		term << (char *)showBuf;
		delete showBuf;
	}		

	parvalue = strlen((const char *)desp->descriptor) + 1;
	term << (XYLocate){parvalue+para->iParameter, COMMAND_LINE};
	
	if(flag & MAKE_DONE)
	{		
		parvalue = 0;
		loop = 0;		
				
		if(para->cParameter!=0)
		{									
			if(flag & PARAMETER_SELECT)
			{
				unsigned short unit;
				do{	
					unit = 1;
					digitallimit = para->cParameter-1;
					while(digitallimit--)
					{
						unit *= 10; 						
						//unit = (unit<<3)+(unit<<1);
					}					
					parvalue += para->parBuffer[loop]*unit;
					loop++;
				}while(--para->cParameter);				
			}
			else
			{
				do
				{				
					parvalue |= para->parBuffer[loop]<<(4*(para->cParameter-1));
					loop++;
				}while(--para->cParameter);				
			}
		}
		
		//update parameter
		exporter->parameter[ctrl->zi.iTaskPar] = parvalue;

		digitallimit = ctrl->zi.iTaskPar;
		if(digitallimit < 4)
		{//update title parameter line
			char *strtemp;
			strtemp = new char[64];
			sprintf( strtemp, "0x%08X", exporter->parameter[ctrl->zi.iTaskPar]);
			menu_title_update(TITLE_PARAMETER_LINE+ctrl->zi.iTaskPar,strtemp);
			delete strtemp;
		}

		if(STATUS_CMP(GO_PAGE_PARAMETER))
		{
			STATUS_CLR(GO_PAGE_PARAMETER|TASK_PAUSE);			
			//if(parvalue > 0x00FFFFFF)
			//pQCEX->TaskStep = 0xFFFFFF;
			//else
			exporter->testStep = parvalue;
		}
		else
		if(STATUS_CMP(TASK_WAIT_ANSWER))
		{
			STATUS_CLR(TASK_WAIT_ANSWER);
			ctrl->zi.iTaskPar++;	
		}
		else
		{
			//ctrl->iTaskPar++;
			//ctrl->cTaskPar--;				
			//if(ctrl->cTaskPar==0)		
			if(ctrl->zi.cTaskPar == ++ctrl->zi.iTaskPar ) 
			{
				STATUS_CLR(TASK_WAIT_PARAMETER);
				menu_task_run_init();
			}
		}		
		
		//update history
		ctrl->historyList[ctrl->cHistory] = parvalue;
		
		if(++ctrl->cHistory == MAX_HISTORY_DEPTH)
		{
			ctrl->cHistory = 0;
		}
		
		ctrl->iHistory = ctrl->cHistory;			
		
		if(STATUS_CMP(TASK_WAIT_PARAMETER))
		{			
			desp++;// = &ctrl->paraInfo[ctrl->iTaskPar];
			menu_show_line(COMMAND_LINE,desp->descriptor);
		}
		
		memset( para, 0, sizeof(ParaTmpBuf) );				
	}
	
#undef UPDATE_PAR//		mBITX(0)
#undef MAKE_DONE	//	mBITX(2)	
#undef PARAMETER_SELECT	//mBITX(3)
}

/**
 *  event on menu key execute when task running phase
 *	@Attribution Private
 *  @param[in]   none
 *  @return      none
 *
 */
static void 
menu_key_when_running(void)
{		

	if(  STATUS_CMP(TASK_WAIT_ANSWER)
	   ||STATUS_CMP(GO_PAGE_PARAMETER))
	{		
		menu_key_when_parameter();
	}
	else
	{
		if(ctrl->keyCode == KEY_ESC)
		{
			ctrl->keyCode = KEY_CANCEL;
			STATUS_SET(TASK_CLOSE);		
		}
		else	
		if(STATUS_CMP(TASK_PAUSE))
		{
			unsigned char key;
			key = ctrl->keyCode;
			switch(key)
			{
				//case KEY_UP:
				case KEY_PAGE_UP:
					if(exporter->testStep!=1)
					{
						exporter->testStep-=2;	
					}
					else	
				default:							
					return;
				//case KEY_DOWN:
				case KEY_PAGE_DOWN:
					break;
				case KEY_HOME:
					exporter->testStep = 0;
					break;
				case KEY_G:
					STATUS_SET(GO_PAGE_PARAMETER);
					menu_question_push( menuMsgLog[logEnterPageNumber], modeDEC, 8 );					
					break;				
			}
			
			if(key!=KEY_G)
			{
				STATUS_CLR(TASK_PAUSE);				
				menu_show_line(COMMAND_LINE,menuMsgLog[logEndOfString]);				
			}
		}
		else
		{
			menu_show_hint(HINT,menuMsgLog[logEndLessTaskHint]);
		}			
	}
}

/**
 *  event on menu key execute flag rise
 *	@Attribution Private
 *  @param[in]   none
 *  @return      -1 if in menu status & press 'Q', it need trigger exit
 *
 */
static unsigned int 
menu_key_on_execute(void)
{
	unsigned int r;
	
	r = 0;
	
	if(STATUS_CMP(WAIT_ANY_KEY))
	{
		STATUS_CLR(WAIT_ANY_KEY);
		menu_screen_redraw();
		//reactive point mark blinking whenever task close
		TimerCtrl().timer_slot_active(activeSlot[SLOT_PMARK_BLINKING], true);
	}
	else
	if(STATUS_CMP(TASK_RUNNING))
	{
		menu_key_when_running();
	}
	else
	if(STATUS_CMP(HELP_VIEWING))
	{
		if(ctrl->keyCode != KEY_ESC)
		{
			menu_show_hint(HINT,menuMsgLog[logHelpViewHint]);	
		}
		else
		{
			STATUS_CLR(HELP_VIEWING);
			menu_screen_redraw();
			TimerCtrl().timer_slot_active(activeSlot[SLOT_PMARK_BLINKING], true);
		}			
	}
	else
	if(STATUS_CMP(TASK_WAIT_PARAMETER))
	{
		menu_key_when_parameter();
	}
	else
	{
		if(ctrl->keyCode == KEY_Q)
		{
			r=-1;
		}
		else
		{
			memu_key_when_menu();
		}
	}
	
	return r;
}


/**
 *  call when task have parameter need input,
 *	@Attribution public
 *  @param[in]   qStr : question string
 *  @param[in]   mode : parameter mode, Hex of Dec
 *  @param[in]   digital : digital number of parameter
 *  @return      none
 *
 */
void 
menu_question_push(TStr qStr, ParameterMode mode, unsigned char digital)
{
	ParaDescriptor *desp;
	char limit;	
	
	if(STATUS_CMP(TASK_RUNNING))
	{//interactive parameter input
		desp = &ctrl->zi.paraInfo[ctrl->zi.iTaskPar];
		menu_show_line(COMMAND_LINE, qStr);		
	}
	else
	{//init task parameter configured		
		desp = &ctrl->zi.paraInfo[ctrl->zi.cTaskPar++];
	}	


	if(digital == 0)
	{
		digital = 1;
	}
	else
	{
		limit=(mode==modeSTR)?(ARGUMENT_DEPTH-1):8;
		if(digital > limit)
		{
			digital = limit;
		}

	}
			
	desp->descriptor=qStr;
	desp->digital=digital;
	desp->mode=mode;
}

/**
 *  menu_set_log_mode
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void 
menu_set_log_mode(void)
{
	STATUS_SET(LOG_MODE);
	TerminalCtrl() << (XYLocate){1, REPORT_LINE(0)};
}
/**
 *  configured low frequency task
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void
menu_set_relaxed_task(unsigned int relaxedInterval)
{
	ctrl->relaxLoad = relaxedInterval;
	STATUS_SET(RELAXED_TASK);
	activeSlot[SLOT_RELAX_TASL_DEC] = TimerCtrl() << (TimerService){ SS_ONE_SHOT, relaxedInterval, NULL };
}

/**
 *  menu main routine, put it in main while(1) loop
 *	@Attribution public
 *  @param[in]   none
 *  @return      -1, 'Q' be press, leave application
 * 				  0, normal exit
 * 				  1, task close exit 
 *
 */
int
menu_main_routine(void)
{
#define available	mBIT(0)
#define notice		mBIT(1)
#define showhint	mBIT(2)
	//unsigned int linkflag;
	unsigned char ihint;
	unsigned char ititle;
	unsigned char flag;
	int ret;
	TerminalCtrl term;
	
	KeyCtrl *osKey;
	osKey = new KeyCtrl();
	if(!osKey->bEmpty)
	{
		KeyCode k;
		*osKey >> k;
		ctrl->keyCode = k.key;
		ctrl->asciiCode = k.ascii;
		STATUS_SET(KEY_EXECTUE);
	}
	delete osKey;
	
	ihint = 0;
	ititle = 0;
	flag = 0;
	ret = 0;
	
	if(STATUS_CMP(KEY_EXECTUE))
	{	
		STATUS_CLR(KEY_EXECTUE);
		flag = menu_key_on_execute();
		if(flag!=0)
		{
			ret = -1;
			goto exit_menu_system;
		}
	}

	flag |= available;
	
	if(STATUS_CMP(RELAXED_TASK))
	{
		if(TimerCtrl().bExpired[activeSlot[SLOT_RELAX_TASL_DEC]])
		{			
			activeSlot[SLOT_RELAX_TASL_DEC] = TimerCtrl()<<(TimerService){SS_ONE_SHOT, ctrl->relaxLoad, NULL};
		}
		else
		{
			flag &= ~available;
		}
	}
			
	if(   (STATUS_CMP(TASK_RUNNING))				//Is Task Running
		&&!(STATUS_CMP(TASK_PAUSE|TASK_CLOSE|TASK_WAIT_ANSWER))	//Is not Task Wait any key to continue
		&& (flag & available ))								//Is Task valid
	{												

		STATUS_SET(TASK_CLOSE);
		
		flag |= showhint;	//
		
		if(ctrl->zi.iActiveTask == 0xFF)
		{
			ctrl->keyCode = KEY_NOT_EXIST;
		}
		else
		{
			//ihint = pQCRoot->pActiveTask((inQC)pQCEX);
			ihint = (ctrl->taskSet[ctrl->zi.iActiveTask].task)(exporter);
			
			ctrl->lastProgress = ihint;
			
			if( ihint <= 100 )
			{//progress information 
				char strtemp[8];				
				sprintf( strtemp, "%3d%s", ihint, "%" );
				menu_title_update(TITLE_PROGRESS_LINE, (TStr)strtemp);
				
				if( ihint < 100 )
				{
					ihint = TEST_CONTINUE;
				}				
			}

			switch(ihint)
			{
				case TEST_FAIL:
					flag |= notice;
				case TEST_PASS:			
					ctrl->keyCode = KEY_COMPLETE;
					break;
				case TEST_ILLEGAL_PARAMETER:
				case TEST_TIME_OUT:
				case TEST_COMPARE_FAIL:
					flag |= notice;
					ctrl->keyCode = KEY_ERROR;
					break;			
				case TEST_MORE_VIEW:
					{
#if 0
						TStr tmpBuf;
						tmpBuf = (TStr)MEMORY_SLOT_GET(64);
						STATUS_SET(TASK_PAUSE);
						sprintf(tmpBuf, menuMsgLog[logGoFunctionFormat],
										ARROW_UP_MARK,ARROW_DOWN_MARK,exporter->testStep);
						mFont_Normal;
						menu_show_line(COMMAND_LINE,tmpBuf);
						MEMORY_SLOT_RELEASE(tmpBuf);
#endif						
					}
				case TEST_WAIT_ANSWER:					
					if(ihint == TEST_WAIT_ANSWER)
					{
						STATUS_SET(TASK_WAIT_ANSWER);
					}
					exporter->testStep++;
				case TEST_CONTINUE:
					STATUS_CLR(TASK_CLOSE);			
				case TEST_DO_NOTHING:
					if(ihint == TEST_DO_NOTHING)
					{
						ctrl->keyCode = KEY_COMPLETE;
					}
					flag &= ~showhint;
					break;			
				case TEST_CANCEL:
					ctrl->keyCode = KEY_CANCEL;								
					break;
				case TEST_CLOSE_MENU:
					system("clear");
					ret = -1;
					goto exit_menu_system;
				default:
					ctrl->keyCode = KEY_UNDEFINE;
					break;
			}
	
//			if( ihint > 99 )
//			{
				ihint -= 100;
//			}

			ihint += logTestPass;																			
		}
	}
			
	if(STATUS_CMP(TASK_CLOSE))
	{
		//move to near wait any key
		//STATUS_MSK(STATUS_MASK);
				
		switch(ctrl->keyCode)
		{			
			case KEY_COMPLETE:
				ititle = logPhaseDone;
				break;
			case KEY_ERROR:
				ititle = logPhaseCancel;
				break;
		//	case KEY_DATA_ABORT:
		//		mShowNotice("un-accessable Address Region!!");
		//		result = PHASE_CRASH;
		//		break;
			case KEY_CANCEL:				
				//mShowHint(QCMessageLog[logForceBreakTask]);
				flag |= showhint;
				ihint = logForceBreakTask;
				ititle = logPhaseCancel;
				break;
			case KEY_NOT_EXIST:
				//mShowNotice(QCMessageLog[logTaskNotAssign]);
				flag |= notice|showhint;
				ihint = logTaskNotAssign;
				ititle = logPhaseTaskNotExist;
				break;
			case KEY_UNDEFINE:
				//mShowHint(QCMessageLog[logUndefineResult]);
				flag |= showhint;
				ihint = logUndefineResult;
				ititle = logPhaseDone;
				break;				
			//default:
			//	break;
			
		}

		if(STATUS_CMP(LOG_MODE))
		{
			term << '\n';
		}
		
		if(flag & showhint)	
		{
			if(flag & notice)
			{
				term << COLOR_NOTICE;
			}
			else
			{
				term << COLOR_FOCUS;
			}
				
			menu_show_line(HINT_LINE,menuMsgLog[ihint]); 
			term << COLOR_NORMAL;
		}
				
		menu_title_update(TITLE_PHASE_LINE,menuMsgLog[ititle]);
				
		//Clear Export and TaskPoint and Task parameter information
		//memset( &ctrl->iActiveTask, 0, sizeof(ParaTmpBuf)+8);
		memset(&ctrl->zi.iActiveTask, 0, sizeof(MenuCtrlZI));
		ctrl->zi.iActiveTask = 0xFF;

		//memset( ctrl->exporter, 0, MEMORY_SLOT_SIZE );

		//Keep the Hint Message
		//STATUS_CLR(HINT_SHOWING);		
		TimerCtrl().timer_slot_active(activeSlot[SLOT_HINT_DISMISS], false);
		menu_show_line(COMMAND_LINE,menuMsgLog[logAnyKeyContinue]);
		STATUS_MSK(STATUS_MASK);
		STATUS_SET(WAIT_ANY_KEY);
		ret = 1;	
	}
exit_menu_system:
	return ret;
}

Progress
menu_direct_task(ValidTask itask, unsigned int op, ...)
{
	int ret;
	va_list list;
		
	TimerCtrl().timer_slot_active(activeSlot[SLOT_PMARK_BLINKING], false);
	
	menu_screen_redraw();
	menu_clr_report(0);
	
	*(int *)exporter = op;
	//exporter->testOP = op;
	//exporter->testStep = 0;
		
	//assign task index
	ctrl->zi.iActiveTask = itask;
	//execute init function, after this, task parameter cnt can be determined
	ctrl->taskSet[itask].init(op);

	va_start(list, op);
	if(ctrl->zi.paraInfo[0].mode == modeSTR)
	{//string mode use strcpy
		strcpy((char *)exporter->arguments, va_arg(list, char *));
	}
	else
	{	
		for(ret=0;ret<ctrl->zi.cTaskPar;ret++)
		{
			exporter->parameter[ret]=va_arg(list, unsigned int);
		}		
	}
	
	va_end(list);
	
	//disable unnecessary flag when direct auto run 
	STATUS_CLR(TASK_WAIT_PARAMETER);
	STATUS_SET(TASK_RUNNING);
	ret = 0;
	//run
	while(0 == ret)
	{		
		KeyCtrl::routine();
		TimerCtrl::routine();
		ret = menu_main_routine();
	}	
	
	if(ret == -1)
	return TEST_CANCEL;
	else
	return (Progress)ctrl->lastProgress;
}

#undef ctrl//	(&menuCtrl)
#undef exporter//	(&ctrl->zi.expotrer)

