#include <stdio.h>
#include "utils.h"
#include "menu_ctrl.h"
#include "menu_system.h"
#include "timer_ctrl.h"

/*===========================================================================*/
//QC MENU String and Attribute Define 
/*===========================================================================*/	
#define REGISTERED_TASK_IDX	TASK_SYSTEM_BASIC
static const char menuSysTBD[] = "TBD";
//Root Menu
static /*const*/ TStr rootMenu[]=
{		
	ITEM_M("Driver operation", MENU_DRIVER_BASIC),
	ITEM_M("IOCTL basic command", MENU_IOCTL_BASIC),
	ITEM_M_IS("NVME admin command", MENU_NVME_ADMIN),
	ITEM_M_IS("NVM command", MENU_NVME_IO),
	ITEM_M_IS("Test tool", MENU_TEST_TOOL),
	//ITEM_M("System Debug", MENU_DEBUG),	
	ITEM_T("EXIT", OP_EXIT_APP),
	__EOM	
};

//Debug Main Menu
static const TStr debugMainMenu[]=
{
	ITEM_T("Progress pass test", OP_PASS_TEST),
	ITEM_T("Progress fail test", OP_FAIL_TEST),
	ITEM_T("parameter test", OP_PARAMETER_TEST),
	ITEM_T("command line", OP_COMMAND_LINE_TEST),
	__EOM	
};

static const TStr defaultMenu[]=
{
	ATTRIBUTE( 0, ATTR_STRING, 0 ),//menuSATAMain),
	(TStr)menuSysTBD,
	__EOM
};

void
root_nvme_menu_swap(int rootSwap)
{
	char attrAdmin=ATTR_STRING;
	char attrSubIO=ATTR_STRING;
	char attrTool=ATTR_STRING;
	switch(rootSwap)
	{
		case ROOT_NVME_MENU_SWAP_NONE:
			//attrAdmin = ATTR_STRING;
			//attrSubIO = ATTR_STRING;
			break;
		case ROOT_NVME_MENU_SWAP_ADMIN:
			attrAdmin = ATTR_FOLDER;
			//attrSubIO = ATTR_STRING;
			break;
		case ROOT_NVME_MENU_SWAP_SUBIO:
			//attrAdmin = ATTR_STRING;
			attrSubIO = ATTR_FOLDER;
			break;
		case ROOT_NVME_MENU_SWAP_BOTH:
			attrAdmin = ATTR_FOLDER;
			attrSubIO = ATTR_FOLDER;
			break;
		case ROOT_NVME_MENU_SWAP_STANDARD:
			attrTool = ATTR_FOLDER;
			break;
		default:
			return;
	}
	
	ChangeItemAttribute(rootMenu, MENU_NVME_ADMIN-1, attrAdmin);
	ChangeItemAttribute(rootMenu, MENU_NVME_IO-1, attrSubIO);
	ChangeItemAttribute(rootMenu, MENU_TEST_TOOL-1, attrTool);	
}

/*===========================================================================*/
//Memu Set
/*===========================================================================*/	
static TStrList menuTable[MENU_MAX_COUNT]=
{
	(TStrList)rootMenu,			//MENU_ROOT
	(TStrList)defaultMenu,		//MENU_DRIVER_BASIC	
	(TStrList)defaultMenu,		//MENU_IOCTL_BASIC
	(TStrList)defaultMenu,		//MENU_NVME_ADMIN
	(TStrList)defaultMenu,		//MENU_NVME_IO
	(TStrList)defaultMenu,		//MENU_TEST_TOOL
	(TStrList)debugMainMenu,	//MENU_DEBUG
};

//static Progress menu_system_default_task(MenuSystemExport *par);
//extern void mem_operate_init(unsigned char mode);
//extern Progress mem_operate(MenuSystemExport* par);

/**
 *  basic memory test init function
 *	@Attribution public
 *  @param[in]   mode : operate mode
 *  @return      none
 *
 */
static void 
system_operate_init(unsigned char mode)
{
	switch(mode)
	{
		case OP_PASS_TEST:
		case OP_FAIL_TEST:
			menu_set_relaxed_task(10);	//10ms
			break;
		case OP_PARAMETER_TEST:
			menu_question_push((TStr)"test 8 digitals HEX parameter: 0x",modeHEX,8);
			menu_question_push((TStr)"test 2 digitals DEC parameter:",modeDEC,2);
			break;
		case OP_COMMAND_LINE_TEST:
			menu_set_log_mode();
			//menu_question_push((TStr)"test string parameter: ",modeSTR,64);
			break;
	}	
}

/**
 *  basic memory test function
 *	@Attribution public
 *  @param[in]   mode : operate mode
 *  @return      none
 *
 */
static Progress 
system_operate(MenuSystemExport* par)
{
	Progress flag;
	unsigned int opmode;
	char *msg;	
	opmode = par->testOP;
	msg = new char[128];
	switch(opmode)
	{
		//case OP_MEMORY_DUMP:
		//	ptask = memory_dump;
		//	break;
		//case OP_MEMORY_WRITE:
		//	ptask = memory_write;
		//	break;
		case OP_PASS_TEST:			
		case OP_FAIL_TEST:
			opmode = par->testStep++;
			if( par->testOP == OP_FAIL_TEST )
			{
				if( opmode == 61 )
				{
					opmode = TEST_FAIL;
				}
			}			
			flag = (Progress)opmode;
			break;
		case OP_PARAMETER_TEST:			
			sprintf(msg, "Hex parameter value = 0x%08X", par->parameter[0]);
			menu_show_report(0, msg);
			sprintf(msg, "DEC parameter value = %02d", par->parameter[1]);
			menu_show_report(1, msg);			
			flag = TEST_PASS;
			break;
		case OP_COMMAND_LINE_TEST:
			if(par->testStep==0)
			{
				menu_question_push((TStr)"enter command :",modeSTR,64);
				flag = TEST_WAIT_ANSWER;				
			}
			else
			{
				system_call((const char *)par->arguments);
				flag = TEST_PASS;
			}
			break;
		case OP_EXIT_APP:
			flag = TEST_CLOSE_MENU;
			break;
		default :
			flag = TEST_ILLEGAL_PARAMETER;
			break;
	}
	
	delete msg;
	return flag;
}

MenuTask taskTable[TASK_MAX_COUNT]=
{	
	{	system_operate_init,	system_operate,	},	//TASK_MEMORY_BASIC
};

/**
 *  default task in table
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
Progress 
menu_system_default_task(MenuSystemExport *par)
{	
	char *msg;
	msg = new char[64];
	sprintf( msg, "test function #%d", par->testOP );
	menu_clr_report(0);
	menu_show_report( 0, msg );
	menu_show_hint( NOTICE, (TStr)"no specific function be registered" );
	delete msg;
	return TEST_DO_NOTHING;
}

/**
 *  menu system init
 *	@Attribution public
 *  @param[in]   none
 *  @return      none
 *
 */
void
menu_system_init(void)
{
	menu_ctrl_init( (TStrListSet)menuTable, (MenuTaskSet)taskTable );	
	//register task;
	ioctl_basic_init();	
	driver_basic_init();
	admin_basic_init();
	submis_basic_init();
	test_tool_init();
	menu_screen_redraw();
}

static const unsigned char taskToMenu[TASK_MAX_COUNT]=
{
		0xFF,//TASK_SYSTEM_BASIC
		MENU_DRIVER_BASIC, //TASK_DRIVER_BASIC
		MENU_IOCTL_BASIC, //TASK_IOCTL_BASIC
		MENU_NVME_ADMIN, //TASK_NVME_ADMIN
		MENU_NVME_IO, //TASK_NVME_IO
		MENU_TEST_TOOL,
				//TASK_MAX_COUNT
};
/**
 *  register a object function menu and its validation task
 *	@Attribution public
 *  @param[in]   index : index of validation task
 *  @param[in]   menu : function item menu 
 *  @param[in]   init : initialize function
 *  @param[in]   task : validation task
 *  @return      none
 *
 */
void
menu_system_task_register( unsigned char index, const TStrList menu, TaskInit init, ValidateTask task )
{
	if( index < TASK_MAX_COUNT )
	{
		unsigned char iMenu;
		taskTable[index].init = init;
		taskTable[index].task = task;
		iMenu = taskToMenu[index];
		if(iMenu!=0xFF)
		{
			menuTable[iMenu]=menu;
		}
		
	}
	
}

