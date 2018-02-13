#ifndef _FIO_TEST_SHELL_H_
#define _FIO_TEST_SHELL_H_
#include "extern_shell.h"

class FioTestShell : public ExternShell
{
public:
	static std::string const shellFile;
	FioTestShell(std::string runTime, std::string testSize);
	~FioTestShell(void);
protected:	
private:
};

#endif //_FILE_CTRL_H_
