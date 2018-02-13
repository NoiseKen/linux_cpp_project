#ifndef _SHELL_SCRIPT_H_
#define _SHELL_SCRIPT_H_
#include <vector>
#include "extern_shell.h"

class ShellScript
{
public:
	enum{
		//Script status define
		SHELL_RUNNING=0,
		SHELL_KILL,
		SHELL_FINISH,
		SHELL_TIMEOUT,
		//const define
		SCRIPT_INFINITE=-1
	};
	bool const blockScript;
	int const timeOut;
	int run_time(void);
	int routine(void);
	int result(void);
	virtual bool is_pass(void) {return (0==result())?true:false;};
	ExternShell * extern_shell(void);	
	virtual void script_log(std::vector<std::string> &list);	
	ShellScript(int to, ExternShell *sh, bool blockWhenFail=false);
	virtual ~ShellScript(void);
protected:	
	virtual int shell_execute(void){_extShell->exec(); return 0;};
	virtual void shell_monitor(int sc){};
	bool expired(void);	
private:
	bool _executed;
	time_t _startTime;
	ExternShell *const _extShell;
};

#endif //_FILE_CTRL_H_
