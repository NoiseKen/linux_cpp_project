#include <errno.h>
#include <string.h>
#include "nvme_tool.h"
#include "terminal_ctrl.h"

#define REGISTERED_TASK_IDX	TASK_NVME_IO
//
static const TStr submisBasicMenu[]=
{	
	ITEM_T("Write", OP_SUBMIS_NVME_WRITE),
	ITEM_T("Read", OP_SUBMIS_NVME_READ),
	ITEM_T("Flush", OP_SUBMIS_NVME_FLUSH),
	ITEM_T("Compare", OP_SUBMIS_NVME_COMPARE),
	__EOM
};


enum{
	LOG_MSG_LBA_H=0,
	LOG_MSG_LBA_L,
	LOG_MSG_NUM_LBA,
	LOG_MSG_PATTERN_RULE,
	LOG_MSG_START_PATTERN,
};

static const char *submisLogMsg[]={
	"Start LBA High = 0x",
	"Start LBA Low = 0x",
	"Number of LBA = ",
	"Pattern Rule = ",
	"Start Pattern = 0x",
};

static Progress
submis_flush(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_user_io *io;
	char *msg;
	int *ptr;
	int ioctlStatus;
	menu_show_report(0,NULL);
	msg = new char[128];
	ptr = (int *)new char[128];
	//ioctl command
	io = new struct nvme_user_io ();
	memset(io, 0, sizeof(struct nvme_user_io));
	io->opcode = nvme_cmd_flush;
	io->control = 1<<15;	//limited retry
	io->addr = (uint64_t)ptr;
	
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_SUBMIS_CMD, io, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);		
		flag = TEST_FAIL;
	}
	else
	{		
		display_status_code(ioctlStatus);			
		flag = TEST_PASS;
	}
	
	delete io;
	delete ptr;
	delete msg;
	return flag;	
}

static Progress
submis_wrc(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_user_io *io;
	int *ptr;
	char *msg;	
	int ioctlStatus;
	unsigned int i, size;
	uint64_t slba;
	int mps;
	TerminalCtrl term;
	
	if(par->parameter[2]==0)
	{
		printf("LBA number cannot be 0!!");
		return TEST_ILLEGAL_PARAMETER;
	}
	
	mps = ioctl_get_controller_mps(true);
	
	if(mps<0)
	{
		//printf("get controller mps fail!!");
		term << "get controller mps fail!!";
		return TEST_FAIL;
	}

	msg = new char[128];
	menu_set_log_mode();
	
	//pattern
	size = (par->parameter[2])*LBA_SIZE;
	ptr = (int *)aligned_malloc(size, mps);
	if(par->testOP!=OP_SUBMIS_NVME_READ)
	{//if write, build pattern
		unsigned int pat;
		pat = par->parameter[4];
		for(i=0;i<size/sizeof(int);i++)
		{
			ptr[i] = pat;
			if(par->parameter[3]==0x01)
			{
				pat++;
			}
			else
			if(par->parameter[3]==0x02)
			{
				pat--;
			}
			else
			if(par->parameter[3]==0x03)
			{
				pat^=0xFFFFFFFF;
			}						
		}
	}
	else
	{//if read, init buffer
		memset(ptr, 0xff, size);
	}
	
	slba = ((uint64_t)par->parameter[0]<<32)|par->parameter[1];
	//ioctl command
	io = new struct nvme_user_io();
	memset(io, 0, sizeof(struct nvme_user_io));
	switch(par->testOP)
	{
		case OP_SUBMIS_NVME_WRITE:
			io->opcode = nvme_cmd_write;
			break;
		case OP_SUBMIS_NVME_READ:
			io->opcode = nvme_cmd_read;
			break;
		case OP_SUBMIS_NVME_COMPARE:
			io->opcode = nvme_cmd_compare;
			break;			
	}
	
	io->control = 1<<15;	//limited retry
	io->nblocks = par->parameter[2]-1;
	io->addr = (uint64_t)ptr;
	io->slba = slba;

	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_SUBMIS_CMD, io, msg);
	if(ioctlStatus < 0)
	{
		//printf("%s\n", msg);
		term << msg << '\n';
		flag = TEST_FAIL;
	}
	else
	{		
		display_status_code(ioctlStatus);			
		flag = TEST_PASS;
	}

	if(par->testOP==OP_SUBMIS_NVME_READ)
	{//if read, dump buffer
		
		for(i=0;i<par->parameter[2];i++)
		{
			term << COLOR_HI_GREEN;
			printf("------------ LBA 0x%016lX -----------\n", slba+i);
			memory_dump((uint64_t)&ptr[i*LBA_SIZE/sizeof(int)], 1);			
		}
	}
	
	delete io;
	aligned_free(ptr);
	delete msg;
	return flag;
}

static void
task_submis_basic_init(unsigned char op)
{	
	menu_clr_report(0);
	menu_show_report(0, (TStr)"");
	switch(op)
	{
		case OP_SUBMIS_NVME_WRITE:
		case OP_SUBMIS_NVME_COMPARE:
			printf("pattern rule :\n");
			printf("0. solid\n");
			printf("1. increment\n");
			printf("2. decrement\n");
			printf("3. toggle\n");
		case OP_SUBMIS_NVME_READ:
			menu_question_push((TStr)submisLogMsg[LOG_MSG_LBA_H], modeHEX, 8);
			menu_question_push((TStr)submisLogMsg[LOG_MSG_LBA_L], modeHEX, 8);
			menu_question_push((TStr)submisLogMsg[LOG_MSG_NUM_LBA], modeDEC, 1);
			if(op!=OP_SUBMIS_NVME_READ)
			{
				menu_question_push((TStr)submisLogMsg[LOG_MSG_PATTERN_RULE], modeHEX, 8);
				menu_question_push((TStr)submisLogMsg[LOG_MSG_START_PATTERN], modeHEX, 8);
			}
			break;		
	}
}


static Progress 
task_submis_basic(MenuSystemExport *par)
{	
	Progress flag;
	
	switch(par->testOP)
	{		
		case OP_SUBMIS_NVME_WRITE:
		case OP_SUBMIS_NVME_READ:
		case OP_SUBMIS_NVME_COMPARE:
			flag = submis_wrc(par);
			break;
		case OP_SUBMIS_NVME_FLUSH:
			flag = submis_flush(par);
			break;
		default:
			flag = TEST_ILLEGAL_PARAMETER;
			break;
	}
	
	return flag;
}

void
submis_basic_init(void)
{	
	menu_system_task_register( REGISTERED_TASK_IDX, submisBasicMenu, task_submis_basic_init, task_submis_basic );
}
