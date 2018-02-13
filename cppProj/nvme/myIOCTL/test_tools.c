#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "nvme_tool.h"
#include "terminal_ctrl.h"
#include "value_list.h"
#include "utils.h"

#define IP_STR_SIZE	(3*4+3+1)

#define REGISTERED_TASK_IDX	TASK_TEST_TOOL

//Debug Main Menu
static const TStr testToolMenu[]=
{
	ITEM_T("IOMeter Test", OP_TEST_IOMETER),
	ITEM_T("Compliance Test", OP_TEST_COMPLIANCE),
	__EOM
};

Progress compliance_test(MenuSystemExport * par)
{
	Progress flag=TEST_FAIL;
	char *cwd;
	std::string managePath;
	std::string compliance;
	char *msg;
	cwd = new char[64];	
	msg = new char[128];
	
	//store current path
	getcwd(cwd, 64);
	//get_config_value(TO_STR(KEY_TEST_TNVME), compliance);
	compliance = ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_TEST_TNVME));
	//switch to compliance test suit path
	//get_config_value(TO_STR(KEY_CMPLMGN_PATH), inBuf);
	managePath = ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_CMPLMGN_PATH));	
	
	if(0==chdir(managePath.c_str()))
	{
		//build compliance command string		
		sprintf(msg, "%s %s", compliance.c_str(), par->arguments);
		if(0==system_call(msg))
		{
			flag = TEST_DO_NOTHING;
		}

		//back to cwd		
		chdir(cwd);
	}

	
	delete cwd;
	delete msg;
	return flag;
}

Progress iometer_test(MenuSystemExport * par)
{
	Progress flag=TEST_DO_NOTHING;
	char *cmdLine;
	std::string iometer;
	char serverIp[IP_STR_SIZE];	
	
	cmdLine = new char[128];
	
	get_ip4_str(serverIp);
	
	iometer = ValueList::get_config_value(APP_CONFIG_FILE_NAME, TO_STR(KEY_TEST_IOMETER));
	
	sprintf(cmdLine, "%s -i %s -m %s", iometer.c_str(), par->arguments, serverIp);
	printf("execute %s\n",cmdLine);

	TerminalCtrl term;
	term << TerminalCtrl::TC_CONFIG_STANDARD;
	system_call(cmdLine);
	term << TerminalCtrl::TC_CONFIG_NO_RESPONSE;
	
	delete cmdLine;
	return flag;
}

static void
task_test_tool_init(unsigned char op)
{
	menu_clr_report(0);
	switch(op)
	{
		case OP_TEST_IOMETER:
			menu_question_push((TStr)"Please enter Client IP4:", modeSTR, IP_STR_SIZE);			
			break;
		case OP_TEST_COMPLIANCE:
			menu_question_push((TStr)"enter argument for compliance test : ", modeSTR, ARGUMENT_DEPTH);			
			break;			
	}
}

static Progress 
task_test_tool(MenuSystemExport *par)
{
	Progress flag;
	menu_set_log_mode();
	switch(par->testOP)
	{
		case OP_TEST_IOMETER:
			flag = iometer_test(par);
			break;
		case OP_TEST_COMPLIANCE:
			flag = compliance_test(par);
			break;			
		default:
			flag = TEST_ILLEGAL_PARAMETER;
			break;	
	}
				
	return flag;
}

void
test_tool_init(void)
{	
	menu_system_task_register( REGISTERED_TASK_IDX, testToolMenu, task_test_tool_init, task_test_tool );
}
