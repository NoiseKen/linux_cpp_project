#include <errno.h>
#include <sys/ioctl.h>
#include "nvme_tool.h"
#include "legacy_string.h"
#include "value_list.h"
#include "terminal_ctrl.h"

#define REGISTERED_TASK_IDX	TASK_DRIVER_BASIC
//Debug Main Menu
static const TStr driverBasicMenu[]=
{
	ITEM_T("replace nvme driver", OP_DRIVER_REPLACE),
	ITEM_T("check driver version", OP_DRIVER_VERSION),
	ITEM_T("list module driver", OP_DRIVER_LIST_MODULE),
	ITEM_T("list device node", OP_DRIVER_LIST_NODE),
	__EOM
};

static Progress
driver_get_version(MenuSystemExport *par)
{
	Progress flag;	
	int ioctlStatus;
	int version;
	char *msg;
	TerminalCtrl term;	
	
	msg = new char[64];
		
	ioctlStatus = ioctl_get_driver_version(&version, msg);
	if(ioctlStatus < 0)
	{					
		printf("%s\n", msg);
		term << COLOR_HI_RED;
		term << "please use correct nvme driver, thanks!!\n";	
		term << COLOR_NORMAL;
		flag = TEST_FAIL;
	}
	else
	{//print controller register		
		printf("Driver Version = %02d.%02d.%04d\n", 
								(version>>24)&0xFF,
								(version>>16)&0xFF,
								(version>>0)&0xFFFF
								);		
		if(version != IOCTL_APP_VER)
		{
			term << COLOR_HI_RED;
			term << "Driver version not match Application!!\n";
			term << COLOR_NORMAL;
		}
								
		flag = TEST_PASS;
	}
	
	delete msg;
	return flag;
}

static Progress
driver_system_call(MenuSystemExport *par)
{
	bool bGrep=false;
	Progress flag=TEST_DO_NOTHING;
	std::string value;
	LegacyString lstr;
	TerminalCtrl term;
	int swap;

	switch(par->testOP)
	{
		case OP_DRIVER_REPLACE:	
			if(par->parameter[0]>2)
			{
				flag = TEST_ILLEGAL_PARAMETER;
				break;
			}
			
			system_call("rmmod nvme.ko");
			
			system_call("rmmod dnvme.ko");			
					
			if(par->parameter[0]==0)
			{//use nxgn driver			
				//get_config_value(TO_STR(KEY_NXGN_DRIVER), value);
				value = ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_NXGN_DRIVER));
				//lstr.sprintf("insmod %s", value);
				lstr << "insmod " << value;
			
				if(0==system_call(lstr.data))
				{
					flag = driver_get_version(par);
					if(flag == TEST_PASS)
					{
						term << "relpace driver success\n";
						lock_function_enable(true);
					}										
				}
				else
				{
					flag = TEST_FAIL;
				}
				
				lstr = "";
				swap = ROOT_NVME_MENU_SWAP_NONE;
			}
			else
			{
				if(par->parameter[0]==1)
				{//use compliance driver
					//get_config_value(TO_STR(KEY_CMPL_DRIVER), value);
					value = ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_CMPL_DRIVER));
					//lstr.sprintf("insmod %s", value);
					lstr << "insmod " << value;
					if(0==system_call(lstr.str().c_str()))
					{
						term << "install compliance driver success\n"
								"current nvme dev list\n";
						lstr = "ls -al /dev";
						bGrep = true;
						flag = TEST_PASS;				
					}
					else
					{
						lstr = "";
						flag = TEST_FAIL;
					}
				}
				else
				{//use standard driver
					if(0==system_call("modprobe nvme"))
					{
						term << "install standard driver success\n"
								"current nvme dev list\n";
						lstr = "ls -al /dev";
						bGrep = true;
					}
					else
					{
						lstr = "";
					}
				
				}
								
				swap = ROOT_NVME_MENU_SWAP_STANDARD;
			}
			
			root_nvme_menu_swap(swap);			
			break;
		case OP_DRIVER_LIST_MODULE:
			lstr = "lsmod";
			bGrep=(par->parameter[0]!=0)?true:false;
			break;
		case OP_DRIVER_LIST_NODE:
			lstr = "ls -al /dev";
			bGrep=(par->parameter[0]!=0)?true:false;
			break;
	}
	
	if(lstr.str()!="")
	{
		if(bGrep)
		{
			lstr += " | grep nvme";
		}
		
		system_call(lstr.data);
	}
	    
    return flag;
}


static void
task_driver_basic_init(unsigned char op)
{
	int iReport=0;
	switch(op)
	{
		case OP_DRIVER_REPLACE:
			menu_show_report(iReport++, (TStr)"associated Driver :");
			menu_show_report(iReport++, (TStr)" 0 : NxGn driver");
			menu_show_report(iReport++, (TStr)" 1 : dnvme, for compliance test"); 
			menu_show_report(iReport++, (TStr)" 2 : standard driver, for generial disk test");
			menu_question_push((TStr)"please select associated driver :", modeDEC, 1);
			break;
		case OP_DRIVER_LIST_MODULE:
		case OP_DRIVER_LIST_NODE:
			menu_question_push((TStr)"only list nvme? (0=NO, others=YES):", modeDEC, 1);
			break;
	}
}

static Progress 
task_driver_basic(MenuSystemExport *par)
{
	Progress flag;	

	menu_set_log_mode();
	
	switch(par->testOP)
	{
		case OP_DRIVER_REPLACE:
		case OP_DRIVER_LIST_MODULE:
		case OP_DRIVER_LIST_NODE:
			flag = driver_system_call(par);
			break;
		case OP_DRIVER_VERSION:
			flag = driver_get_version(par);
			break;
		default:
			flag = TEST_ILLEGAL_PARAMETER;
			break;	
	}
				
	return flag;
}

void
driver_basic_init(void)
{	
	menu_system_task_register( REGISTERED_TASK_IDX, driverBasicMenu, task_driver_basic_init, task_driver_basic );
}
