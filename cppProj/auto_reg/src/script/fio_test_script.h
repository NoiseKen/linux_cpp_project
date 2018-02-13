#ifndef _FIO_TEST_SCRIPT_H_
#define _FIO_TEST_SCRIPT_H_
#include "fio_test_shell.h"
#include "shell_script.h"


class FioTestScript : public ShellScript
{
public:
	void script_log(std::vector<std::string> &list);
	FioTestScript(std::string runTime="0", std::string testSize="0");
	~FioTestScript(void);
protected:
	void shell_monitor(int sc);
private:
	static FioTestShell *_new_shell(std::string runTime, std::string testSize);
};

#endif //_FILE_CTRL_H_
