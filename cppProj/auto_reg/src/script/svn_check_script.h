#ifndef _SVN_CHECK_SCRIPT_H_
#define _SVN_CHECK_SCRIPT_H_
#include "svn_check_shell.h"
#include "shell_script.h"


class SVNCheckScript : public ShellScript
{
public:
	static std::string const morphResult;
	bool is_pass(void);
	void script_log(std::vector<std::string> &list);
	SVNCheckScript(std::string localBin);
	~SVNCheckScript(void);
protected:
	void shell_monitor(int sc);
private:
	static SVNCheckShell *_new_shell(std::string localBin);
};

#endif //_FILE_CTRL_H_
