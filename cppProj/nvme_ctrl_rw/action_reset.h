#ifndef _ACTION_RESET_H_
#define _ACTION_RESET_H_
#include "action_base.h"
class ActionReset : public ActionBase
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(std::string path);
    void help_action_example(void);
    ActionReset(int argc, char* argv[]);
    virtual ~ActionReset(void);
protected:
private:
    enum{
        ACTION_RESET_HELP=0,
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _ACTION_RESET_H_
