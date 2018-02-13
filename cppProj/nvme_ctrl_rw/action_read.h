#ifndef _ACTION_READ_H_
#define _ACTION_READ_H_
#include "action_base.h"
class ActionRead : public ActionBase
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(std::string path);
    void help_action_example(void);
    ActionRead(int argc, char* argv[]);
    virtual ~ActionRead(void);
protected:
private:
    int __len;
    int __addr;
    enum{
        ACTION_READ_ADDR=0,
        ACTION_READ_LEN,
        ACTION_READ_HELP
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _ACTION_READ_H_
