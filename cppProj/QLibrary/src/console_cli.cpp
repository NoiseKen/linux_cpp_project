#include <stdlib.h>
#include <QTime>
#include "utils_qt.h"
#include "terminal_ctrl.h"
#include "debug.h"
#include "console_cli.h"

const QString ConsoleCLI::hintBackToGUI="--- Press any key back to GUI ---";
const int ConsoleCLI::hintFlashInterval=500;


unsigned int 
ConsoleCLI::_f_get_command_line(void)
{
	XYLocate res = TerminalCtrl().resolution;
	return res.y;
}

unsigned int 
ConsoleCLI::_f_get_hint_line(void)
{
	XYLocate res = TerminalCtrl().resolution;
	return res.y-1;
}

int
ConsoleCLI::run(QString cli)
{
	int i;
	//QString newConsole = "gnome-terminal --maximize -e ";
	TerminalCtrl *term;
	term = new TerminalCtrl();

	*term << TerminalCtrl::TC_CONFIG_NO_ECHO;
	*term << AnsiGen::EC_CLR_SCREEN << COLOR_HI_GREEN << (XYLocate){1, 1};
	*term << "***************************************************************\n";
	*term << "** NxGn NVMe Host App -- Console mode --\n";
	*term << "***************************************************************\n";
	*term << COLOR_NORMAL << QString("Executing Cmd:\n  %1").arg(cli).toStdString();
	*term << "\n ------------------------- OUTPUT --------------------\n" << TerminalCtrl::TC_FLUSH;	
	
	i = system(cli.toLocal8Bit().data());
	*term << TerminalCtrl::TC_CONFIG_NO_RESPONSE;
	XYLocate res;
	res.x = 1; res.y=hintLine;
	*term << res;
	if(i==0)
	{
		*term << COLOR_FOCUS << AnsiGen::EC_CLR_AFTER << "TEST PASS";
	}
	else
	{
		*term << COLOR_NOTICE << AnsiGen::EC_CLR_AFTER <<"TEST FAIL";
	}
	
	*term << TerminalCtrl::TC_FLUSH << COLOR_NORMAL;
	
	UtilsQT::flush_stdin(false);
	QTime *time;
	time = new QTime();
	time->start();
	do{
		QString msg;
		if(time->elapsed()>hintFlashInterval)
		{
			time->restart();
			msg = (bShowHint)?hintBackToGUI:"";			
			bShowHint ^= true;
		
			XYLocate res;// = term->resolution;
			res.x = 1; res.y=commandLine;
			TerminalCtrl() << res << AnsiGen::EC_CLR_AFTER << msg.toStdString() << TerminalCtrl::TC_FLUSH;
		}
	}while(0==UtilsQT::stdin_ready_byte());
	UtilsQT::flush_stdin(false);
	
	delete time;
	
	return i;	
}

ConsoleCLI::ConsoleCLI(void) : hintLine(this, &ConsoleCLI::_f_get_hint_line)
				,commandLine(this, &ConsoleCLI::_f_get_command_line)
{
	bShowHint=true;
}

ConsoleCLI::~ConsoleCLI(void)
{
}
