#include "system_task.h"
#include "debug.h"
#include "io_ctrl.h"

SystemTask::SystemTask(void) : state(this, &SystemTask::_f_get_current_state)
					,stdOut(this, &SystemTask::_f_get_std_out)
{
	process = new QProcess();
	process->setProcessChannelMode(QProcess::MergedChannels);
	exitCode = systemTaskCancel;
}

SystemTask::~SystemTask(void)
{	
	if(!process->waitForFinished(3000))
	{
		process->kill();
	}	
	delete process;
}

void 
SystemTask::delay(unsigned int ms)
{
	msleep(ms);
}


QString 
SystemTask::_f_get_std_out(void)
{
	QString rep;
	repCh >> rep;
	return rep;
}



QProcess::ProcessState 
SystemTask::_f_get_current_state(void)
{			
	return process->state();
}


void SystemTask::run(void)
{
#if 0	
	while(QProcess::Running==process->state())
	{
		setTerminationEnabled(false);				
		if(process->canReadLine())
		//if(process->bytesAvailable())
		//if(!process->atEnd())
		{					
			repCh << QString::fromAscii(process->readLine());
		}
		setTerminationEnabled(true);
	}
	
	exitCode = process->exitCode();
	repCh << QString().sprintf("exit code = %d\n", exitCode);
#else
	while(QProcess::Running==process->state())
	{
		setTerminationEnabled(false);				
		
		//if(process->bytesAvailable())
		{					
			repCh << QString::fromAscii(process->readAllStandardOutput());
		}
		setTerminationEnabled(true);
	}
		
	exitCode = process->exitCode();
	repCh << QString::fromAscii(process->readAll());
	repCh << QString().sprintf("exit code = %d\n", exitCode);
#endif	
}

void
SystemTask::call(QString cmd, QStringList arg)
{
	process->start(cmd, arg);	
	process->waitForStarted();
}

