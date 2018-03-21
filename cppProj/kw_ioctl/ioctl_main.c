#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "terminal_ctrl.h"
#include "key_ctrl.h"
#include "timer_ctrl.h"
#include "menu_ctrl.h"
#include "nvme_tool.h"


void app_system_init(void)
{
	TimerCtrl() << (TimerService){ SS_ACTIVE, 1000/20, TerminalCtrl::flush };
}

extern Progress driver_basic_direct_replace(void);
extern Progress ioctl_basic_direct_auto_lock_first(int init);

static int 
arg_decode(char *arg)
{
	char *parStr;
	int parInt;
	int status;
	status = 0;
	Progress prog;

	if(arg[0]=='a' && arg[2]==__EOS)
	{//auto function
		parStr = &arg[1];		
		parInt = atoi(parStr);
		switch(parInt)
		{	//KW driver
			case 0x00:	//test admin command
			case 0x01:	//test nvme command
				prog = menu_direct_task(TASK_DRIVER_BASIC, OP_DRIVER_REPLACE, 0);
				if(prog == TEST_PASS)								
				{					
					prog = menu_direct_task(TASK_IOCTL_BASIC, OP_IOCTL_AUTO_LOCK_FIRST
										, 0	//VID
										, 0 //DID
										, parInt	//init
									);
				}
				
				if(prog != TEST_PASS)
				{
					status = -1;					
				}
				break;			
			case 0x02:	//Compliance driver
			case 0x03:	//standard driver
				menu_direct_task(TASK_DRIVER_BASIC, OP_DRIVER_REPLACE, parInt-1);
				break;			
		}
	}
	
	return status;
}
#include "legacy_string.h"
int main(int argc, char *argv[])
{	
	TerminalCtrl term;
	
	app_system_init();
	
	menu_system_init();
	
	term << TerminalCtrl::TC_CONFIG_NO_RESPONSE;
	//argument proc
	if( argc > 1 )
	{	
		int i;							
		for(i=1;i<argc;i++)
		{
			arg_decode(argv[i]);			
		}		
	}
	//main wile 1 loop
	while(-1 != menu_main_routine())
	{		
		KeyCtrl::routine();
		TimerCtrl::routine();
	}
	
	term << AnsiGen::EC_CLR_SCREEN << COLOR_HI_GREEN;
	term << "\nBYE BYE!!\n";
	term << COLOR_NORMAL;
		
	term << TerminalCtrl::TC_CONFIG_STANDARD;
		
    return 0;    

}
