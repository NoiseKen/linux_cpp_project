#include <unistd.h>
#include <assert.h>
#include "shell_script.h"

int 
ShellScript::run_time(void)
{
	time_t delta;
	delta = time(NULL) - _startTime;
	return (int)delta;
}

void 
ShellScript::script_log(std::vector<std::string> &list)
{
	list.resize(0);
}

int 
ShellScript::routine(void)
{
	int sc;
	if(!_executed)
	{
		_executed = true;
		_startTime = time(NULL);
		shell_execute();
		sc = SHELL_RUNNING;
		//wait for shell running file be establish
		while(!extern_shell()->is_running());
	}
	else
	{		
		sc = (_extShell->is_running())?SHELL_RUNNING:SHELL_FINISH;		
		shell_monitor(sc);
		if(SHELL_RUNNING==sc)
		{
			if(expired())
			{
				sc = SHELL_TIMEOUT;
				_extShell->kill();				
			}
		}
	}
	return sc;
}

bool 
ShellScript::expired(void)
{
	bool exp=false;
	if(timeOut>0)
	{
		time_t delta;
		delta = time(NULL) - _startTime;
		exp=(delta>timeOut)?true:false;
	}
	return exp;
}

int 
ShellScript::result(void)
{
	return _extShell->result_num();
}

ExternShell * 
ShellScript::extern_shell(void)
{
	return _extShell;
}

ShellScript::ShellScript(int to, ExternShell *sh, bool blockWhenFail)
	: blockScript(blockWhenFail), timeOut(to)
	, _extShell(sh)
{
	_executed = false;
}

ShellScript::~ShellScript(void)
{
	delete _extShell;
}
