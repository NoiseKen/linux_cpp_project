#ifndef _ACTION_DUMP_H_
#define _ACTION_DUMP_H_
#include "action_base.h"
class ActionDump : public ActionBase
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(std::string path);
    void help_action_example(void);
    ActionDump(int argc, char* argv[]);
    virtual ~ActionDump(void);
protected:
    std::string _dumpFile;
private:
    enum{
        ACTION_DUMP_FILE=0,
        ACTION_DUMP_HELP,
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _ACTION_RESET_H_
