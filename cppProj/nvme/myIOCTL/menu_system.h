#ifndef _MENU_SYSTEM_H_
#define _MENU_SYSTEM_H_

#include <stdlib.h>
#include <stdint.h>
#include "utils.h"
#include "op_collect.h"


//#define MEMORY_ALLOCATE		//m3_mem_allocate
//#define MEMORY_SLOT_GET		malloc//m3_mem_get_free_slot
//#define MEMORY_SLOT_RELEASE	free	//m3_mem_release_slot

#define ITEM_M(des, mi)		ATTRIBUTE( 0, ATTR_FOLDER, mi ), (TStr)des
#define ITEM_T(des, op)		ATTRIBUTE( op, ATTR_TASK, REGISTERED_TASK_IDX ), (TStr)des
#define ITEM_S(des)			ATTRIBUTE( 0, ATTR_STRING, 0 ), (TStr)des
//menu init string
#define ITEM_M_IS(des, mi)	ATTRIBUTE( 0, ATTR_STRING, mi ), (TStr)des
//task init string
#define ITEM_T_IS(des, op)	ATTRIBUTE( op, ATTR_STRING, REGISTERED_TASK_IDX ), (TStr)des


//---- TASK_MEMORY_BASIC operation ------
enum{
	OP_MEMORY_DUMP=0,
	OP_MEMORY_WRITE,
	OP_PASS_TEST,
	OP_FAIL_TEST,
	OP_PARAMETER_TEST,	
	OP_COMMAND_LINE_TEST,	
	OP_EXIT_APP,
};

//---- show hint ------------
#define HINT	0
#define NOTICE	1

//----------- menu index -----
typedef enum { 
	MENU_ROOT = 0,
	MENU_DRIVER_BASIC,
	MENU_IOCTL_BASIC,
	MENU_NVME_ADMIN,
	MENU_NVME_IO,
	MENU_TEST_TOOL,
	MENU_DEBUG,
	MENU_MAX_COUNT 
}vaildMenu;

//----------- task index -----
typedef enum { 
	TASK_SYSTEM_BASIC = 0,
	TASK_DRIVER_BASIC,
	TASK_IOCTL_BASIC,
	TASK_NVME_ADMIN,
	TASK_NVME_IO,
	TASK_TEST_TOOL,
	TASK_MAX_COUNT
}ValidTask;

//----------- task index -----
typedef enum { 
	//0~99 for test progress menu system module need update the progress information
	TEST_PASS = 100,
	TEST_FAIL, TEST_ILLEGAL_PARAMETER, TEST_TIME_OUT, TEST_COMPARE_FAIL, TEST_CANCEL,
	TEST_MORE_VIEW, TEST_WAIT_ANSWER, TEST_DO_NOTHING, TEST_CONTINUE,
	//close menu
	TEST_CLOSE_MENU,
}Progress;

typedef enum{
	modeHEX, modeDEC, modeSTR,
}ParameterMode;

#define ARGUMENT_DEPTH			64
#define MAX_PARAMETER_DEPTH		ARGUMENT_DEPTH/sizeof(int)

typedef struct _MenuExport
{							
	//32 separate--------------------------	
	unsigned int testOP:8;			//operation for task
	unsigned int testStep:24;			//Step Count	
	union{	
	unsigned char arguments[ARGUMENT_DEPTH];
	unsigned int parameter[];
	};
} __attribute__((__packed__)) MenuSystemExport;

typedef Progress(*ValidateTask)(MenuSystemExport*);	//for QC_TASK_PAR
typedef void(*TaskInit)(unsigned char);				//for special flag

typedef struct{
	TaskInit init;
	ValidateTask task;
} __attribute__((__packed__)) MenuTask, *MenuTaskSet;

#define SpecialFlag(arg)		((arg&0xFF)<<16)
#define Attribute(arg)			((arg&0xFF)<<8)
	#define ATTR_FOLDER			0xFF
	#define ATTR_STRING			0xFE
	#define ATTR_TASK			0x00	
#define SubIndex(arg)			((arg&0xFF)<<0)
#define ATTRIBUTE(Sf,At,Si)	(TStr)(SpecialFlag(Sf)|Attribute(At)|SubIndex(Si))

#define TITEM(op,desc) 	ATTRIBUTE( op, ATTR_TASK, TASK_INDEX ),	desc

extern void menu_system_init(void);
extern Progress menu_system_default_task(MenuSystemExport *par);
extern void menu_system_task_register( unsigned char index, const TStrList menu, TaskInit init, ValidateTask task );
extern void menu_show_report(unsigned char index ,TStr showstring);
extern void menu_show_hint(unsigned char horn ,TStr showstring);
extern void menu_clr_report(unsigned char ireport);
extern void menu_question_push(TStr qStr, ParameterMode mode, unsigned char digital);
extern void menu_set_relaxed_task(unsigned int relaxedInterval);
extern void menu_set_log_mode(void);
//extern void menu_log_configure( unsigned int iLog );
//extern unsigned int menu_log( unsigned int iLog, TStr msg );
extern void ChangeItemAttribute(TStrList ptrMenuList, unsigned int idxItem, unsigned char attr);
extern void memory_dump(uint64_t raddr, unsigned int nLBA);
extern Progress menu_direct_task(ValidTask itask, unsigned int op, ...);
#endif//ndef _QC_SYSTEM_H_
