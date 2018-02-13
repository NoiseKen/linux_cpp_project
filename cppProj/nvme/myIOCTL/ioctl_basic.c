#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "nvme_tool.h"
#include "terminal_ctrl.h"
#include "value_list.h"
#include "io_ctrl.h"

#define REGISTERED_TASK_IDX	TASK_IOCTL_BASIC
//
static /*const*/ TStr ioctlBasicMenu[]=
{
	ITEM_T("Auto Scan/ Lock 1st NVMe Device", OP_IOCTL_AUTO_LOCK_FIRST),
	ITEM_T("Scan PCI Device", OP_IOCTL_SCAN_PCI),
	ITEM_T("Lock PCI Device", OP_IOCTL_LOCK_PCI),
	ITEM_T("Read NVMe controller register", OP_IOCTL_GET_NVME_CR),
	//ITEM_T("Test vendor ioctl", OP_IOCTRL_TEST),
	ITEM_S("This App only can lock one PCI device,"),
	ITEM_S("if you want to lock others or relock current device,"),
	ITEM_S("please execute replace the nvme driver again."),
	__EOM
};


int 
ioctl_nvme_pass_through(ValidPassThroughType type, void *cbw, TStr msg)
{
	int status=-EINVAL;
	struct{
		unsigned int t2c;
		const char *t2k;
	}pt[PASS_THROUGH_TYPE_MAX]={
		{NVME_IOCTL_VENDOR_CMD, TO_STR(KEY_NODE_DEV)},
		{NVME_IOCTL_ADMIN_CMD, TO_STR(KEY_NODE_DEV)},
		{NVME_IOCTL_SUBMIT_IO, TO_STR(KEY_NODE_BLK)},
	};
		
	if(type<PASS_THROUGH_TYPE_MAX)
	{
		std::string node;
		IOCtrl *ioCtrl;
		status =0;

		node = ValueList::get_config_value(APP_CONFIG_FILE_NAME, pt[type].t2k);
		ioCtrl = new IOCtrl(node);
		if (!ioCtrl->bConnected)
		{
			if(msg != NULL)
			{
				sprintf(msg, "open node %s failed", node.c_str());
			}
			status = -1;
		} 
		else
		{
			status = ioCtrl->pass_through(pt[type].t2c, cbw);
			if((status < 0)&&(msg != NULL))
			{			
				sprintf(msg, "IOCTL issue fail, errno = %d", errno);
			}
		}

		delete ioCtrl;
    }
    
    return status;		
}

void
lock_function_enable(bool en)
{
	unsigned char attr;
	if(en)
	{
		attr = ATTR_TASK;
	}
	else
	{
		attr = ATTR_STRING;
	}	
	ChangeItemAttribute(ioctlBasicMenu, OP_IOCTL_AUTO_LOCK_FIRST, attr);
	ChangeItemAttribute(ioctlBasicMenu, OP_IOCTL_LOCK_PCI, attr);		
}

int
ioctl_get_driver_version(int *version, TStr msg)
{
	struct nvme_vendor_cmd vendor;
	int ioctlStatus;

	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor.cmd = IOCTL_VENDOR_GET_VERSION;
	vendor.result = version;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg);
	return ioctlStatus;
}

int 
ioctl_get_controller_register(struct nvme_bar *bar, TStr msg)
{
	int ioctlStatus;
	struct nvme_vendor_cmd vendor;
	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor.cmd = IOCTL_VENDOR_READ_NVME_CR;
	vendor.result = bar;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg);
	return ioctlStatus;
}

/*
 * function	: ioctl_get_controller_mps
 * 			  get locked nvme controller maximum page size
 * parameter: [powOf2]=true,return value will be power of 2
 * 					  =false, return value will be integer
 * return	: > 0 mean mps, < 0 mean linux error code 
 */
int
ioctl_get_controller_mps(bool powOf2)
{
	struct nvme_bar bar;
	int result;
	
	result = ioctl_get_controller_register(&bar, NULL);
	
	if(result==0)
	{
		result = (((bar.cc>>7)&0x0F)+12);
		if(powOf2==false)
		{
			result <<= 1;
		}
	}
	
	return result;
}

static Progress
ioctl_read_nvme_ctrl_reg(MenuSystemExport *par)
{
#define CC_MPS	((bar->cc>>7)&0x0F)
	Progress flag;
	struct nvme_bar *bar;
	int ioctlStatus;
	char *msg;
	menu_set_log_mode();
	msg = new char[64];
	bar = new struct nvme_bar();
	ioctlStatus = ioctl_get_controller_register(bar, msg);
	if(ioctlStatus < 0)
	{
		printf("%s\n", msg);
		flag = TEST_FAIL;
	}
	else
	{//print controller register
		unsigned int i32;
		printf("CAP   = 0x%016lX\n", bar->cap);
		printf("VS    = 0x%08X\n", bar->vs);
		printf("IMTMS = 0x%08X\n", bar->intms);		
		printf("IMTMC = 0x%08X\n", bar->intmc);		
		printf("CC    = 0x%08X\n", bar->cc);
		printf("CSTS  = 0x%08X\n", bar->csts);
		printf("NSSR  = 0x%08X\n", bar->nssr);		
		printf("AQA   = 0x%08X\n", bar->aqa);
		printf("ASQ   = 0x%016lX\n", bar->asq);
		printf("ACQ   = 0x%016lX\n", bar->acq);		
		printf("------detail------\n");		
		i32 = bar->cap & 0xFFFF;
		printf("CAP.MQES = 0x%04X => maximum %d enteries\n", i32, i32+1);
		i32 = bar->vs;
		printf("NVMe version = %d.%d\n", (i32>>16), i32&0xFFFF);
		i32 = (bar->cc>>20)&0x0F;
		printf("CC.IOCQES = 0x%02X ==> I/O completion queue entry size = %d\n", i32, 1<<i32);
		i32 = (bar->cc>>16)&0x0F;
		printf("CC.IOSQES = 0x%02X ==> I/O submission queue entry size = %d\n", i32, 1<<i32);		
		i32 = ((bar->cc>>7)&0x0F);
		printf("CC.MPS = 0x%02X ==> memory page size = %d\n", i32, 1<<(i32+12));		
		i32 = bar->cc&0x01;
		printf("CC.EN= %d\n", i32);
		i32 = (bar->csts>>0)&0x01;
		printf("CSTS.RDY= %d\n", i32);
		
		
		flag = TEST_PASS;
	}
	delete bar;
	delete msg;
	return flag;
#undef CC_MPS	//((bar->cc>>7)&0x0F)
}

static Progress
ioctl_test(MenuSystemExport *par)
{
	Progress flag;
	struct nvme_vendor_cmd vendor;
	int ioctlStatus;

	char *msg;
	msg = new char[64];

	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor.cmd = IOCTL_VENDOR_TEST;
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg);
	if(ioctlStatus < 0)
	{					
		menu_show_report(0, msg);
		flag = TEST_FAIL;		
	}
	else
	{
		flag = TEST_PASS;
	}
	
	delete msg;
	
	return flag;
}

static Progress
ioctl_lock_pci_device(MenuSystemExport *par)
{
#define cmdCBW	(&vendor.cbw.lockPciDev)
	Progress flag;
	struct nvme_vendor_cmd vendor;
	int classCode;	
	int ioctlStatus;
	char *msg;
	msg = new char[64];

	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));	
	vendor.cmd = IOCTL_VENDOR_LOCK_PCI;
	vendor.result = &classCode;
	cmdCBW->targetNumber = par->parameter[0];
	cmdCBW->vendorId = par->parameter[1];
	cmdCBW->deviceId = par->parameter[2];
	cmdCBW->initialize = (par->parameter[3]==0)?false:true;
	
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg);
	if(ioctlStatus < 0)
	{
		if(errno==EPERM)
		{
			sprintf(msg, "Class code 0x%08X not match NVMe class(0x%08X)!!"
								,classCode, PCI_CLASS_STORAGE_EXPRESS);
		}
		flag = TEST_FAIL;
	}
	else
	{		
		sprintf(msg, "NVMe Device locked!!");
		//one time exectuable
		lock_function_enable(false);
		if(par->parameter[3]!=0)
		{//Init dev
			root_nvme_menu_swap(ROOT_NVME_MENU_SWAP_BOTH);
			admin_cmd_menu_swap(ADMIN_MENU_STANDARD);
		}
		else
		{//map bar only
			root_nvme_menu_swap(ROOT_NVME_MENU_SWAP_ADMIN);
			admin_cmd_menu_swap(ADMIN_MENU_SWAP_QUEUE_INIT);
		}
		
		flag = TEST_PASS;
	}	
	menu_show_report(0, msg);	
	delete msg;
	return flag;
#undef cmdCBW//	(&vendor.cbw.scanPciDev)
}

static Progress
get_pci_dev_list(int vid, int did, TStr msg, AutoScanPci *list)
{
#define cmdCBW	(&vendor.cbw.scanPciDev)	
	Progress flag;
	struct nvme_vendor_cmd vendor;
	int ioctlStatus;
	memset(&vendor, 0, sizeof(struct nvme_vendor_cmd));
	vendor.cmd = IOCTL_VENDOR_SCAN_PCI;
	vendor.result = list;
	cmdCBW->vendorId = vid;
	cmdCBW->deviceId = did;	
	ioctlStatus = ioctl_nvme_pass_through(PASS_THROUGH_DEV_VENDOR, &vendor, msg);

	flag = (ioctlStatus<0)?TEST_FAIL:TEST_PASS;
	return flag;
#undef cmdCBW
}

static Progress
ioctl_auto_lock_first(MenuSystemExport *par)
{
	Progress flag;
	AutoScanPci *list;
	int ioctlStatus;
	char *msg;
	msg = new char[64];	
	list = (AutoScanPci *)new char[8192];
	memset(list, 0xFF, 8192);
	ioctlStatus = get_pci_dev_list(par->parameter[0], par->parameter[1], msg, list);
	
	if(ioctlStatus < 0)
	{
		flag = TEST_FAIL;
	}
	else
	{//now find 1st NVMe dev and lock it		
		unsigned int i;	
		bool locked=false;
		
		if(list->cnt != 0)
		{
			i=0;				
			do{
				if(list->info[i].classCode==PCI_CLASS_STORAGE_EXPRESS)
				locked = true;
			}while(!locked&&(++i!=list->cnt));
		}
		
		if(locked)
		{
			int vid, did;
			vid = par->parameter[0];
			did = par->parameter[1];
			par->parameter[3] = par->parameter[2];
			
			par->parameter[0] = i;	//device count
			par->parameter[1] = vid;
			par->parameter[2] = did;
			
			flag = ioctl_lock_pci_device(par);
			if(flag == TEST_PASS)
			{
				sprintf(msg, "\nlocked %03d: VID=0x%04X, DID=0x%04X, class code = 0x%08X", i,
						list->info[i].vid, list->info[i].did, list->info[i].classCode);
			}
		}
		else
		{
			sprintf(msg, "no NVMe deivce can be locked!!");
			flag = TEST_FAIL;
		}
	}	
	printf("%s\n", msg);
		
	delete list;
	delete msg;
	return flag;
}

static Progress
ioctl_scan_pci_device(MenuSystemExport *par)
{
	Progress flag;
	AutoScanPci *list;
	int ioctlStatus;
	char *msg;
	TerminalCtrl term;
	msg = new char[64];
	list = (AutoScanPci *)new char[8192];
	memset(list, 0xFF, 8192);
	ioctlStatus = get_pci_dev_list(par->parameter[0], par->parameter[1], msg, list);
	if(ioctlStatus < 0)
	{
		flag = TEST_FAIL;
	}
	else
	{			
		unsigned int i;
		for(i=0;i<list->cnt;i++)
		{			
			sprintf(msg, "%03d: VID=0x%04X, DID=0x%04X, class code = 0x%08X\n", i, 
						list->info[i].vid, list->info[i].did, list->info[i].classCode);
			
			if(list->info[i].classCode==PCI_CLASS_STORAGE_EXPRESS)
			term << COLOR_HI_GREEN;
									
			term << msg << COLOR_NORMAL;
		}

		flag = TEST_PASS;
		sprintf(msg, "total %d pci device(s) found in this platform", list->cnt);
	}	
	printf("%s\n", msg);

	delete list;
	delete msg;
	return flag;
}

enum{
	IOCTL_MSG_VID=0,
	IOCTL_MSG_DID,
};
static const char *ioctlMsg[]={
	"Vendor ID (0=PCI_ANY_ID)=0x",
	"Device ID (0=PCI_ANY_ID)=0x",
};

static void
task_ioctrl_basic_init(unsigned char op)
{
	switch(op)
	{		
		case OP_IOCTL_SCAN_PCI:
			menu_question_push((TStr)ioctlMsg[IOCTL_MSG_VID], modeHEX, 4);
			menu_question_push((TStr)ioctlMsg[IOCTL_MSG_DID], modeHEX, 4);
			break;
		case OP_IOCTL_LOCK_PCI:
			menu_question_push((TStr)"lock number of device:", modeDEC, 3);
		case OP_IOCTL_AUTO_LOCK_FIRST:
			menu_question_push((TStr)ioctlMsg[IOCTL_MSG_VID], modeHEX, 4);
			menu_question_push((TStr)ioctlMsg[IOCTL_MSG_DID], modeHEX, 4);
			menu_question_push((TStr)"initilize NVMe Device ? (0=NO, others=YES):", modeDEC, 1);			
			break;		
	}
}

static Progress 
task_ioctl_basic(MenuSystemExport *par)
{
	Progress flag;
	ValidateTask task=NULL;
	if(0!=ioctl_get_driver_version((int *)&flag, NULL))
	{
		menu_show_report(0, (TStr)"driver not resopnse ioctl, please replace correct one!!");
		return TEST_CANCEL;
	}
	
	switch(par->testOP)
	{
		case OP_IOCTL_AUTO_LOCK_FIRST:
				menu_set_log_mode();
				task = ioctl_auto_lock_first;
				break;
		case OP_IOCTL_SCAN_PCI:		
				menu_set_log_mode();
				task = ioctl_scan_pci_device;
				break;
		case OP_IOCTL_LOCK_PCI:
				task = ioctl_lock_pci_device;
				break;
		case OP_IOCTL_GET_NVME_CR:
				task = ioctl_read_nvme_ctrl_reg;
				break;
		case OP_IOCTRL_TEST:
				task = ioctl_test;
				break;				
		default:
				flag = TEST_ILLEGAL_PARAMETER;
				break;
	}
	
	if(task!=NULL)
	{
		flag = task(par);
	}
	
	return flag;
}

void
ioctl_basic_init(void)
{	
	menu_system_task_register( REGISTERED_TASK_IDX, ioctlBasicMenu, task_ioctrl_basic_init, task_ioctl_basic );
}
