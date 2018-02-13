#ifndef _INIT_CARD_SCRIPT_H_
#define _INIT_CARD_SCRIPT_H_
#include "initialize_card_shell.h"
#include "shell_script.h"


class InitCardScript : public ShellScript
{
public:
	static std::string const morphResult;
	void script_log(std::vector<std::string> &list);
	InitCardScript(int to);
	~InitCardScript(void);
protected:
	void shell_monitor(int sc);
private:
	static InitializeCardShell *_new_shell(void);
};

#endif //_FILE_CTRL_H_
