#ifndef _ACTION_BASE_H_
#define _ACTION_BASE_H_
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "console_arguments.h"

class ActionBase : public ConsoleArguments
{
public:
    std::string const cmdString;
    virtual bool is_help(void)=0;
    virtual bool is_valid(void)=0;    
    virtual int issue(std::string path)=0;
    virtual void post(void){};
    virtual void help_action_example(void){};
    ActionBase(int argc, char* argv[], std::string cmdStr, GlobalVariable *const pVars, int varsSize);
    virtual ~ActionBase(void);
protected:
private:
};
#endif //ndef _ACTION_BASE_H_
