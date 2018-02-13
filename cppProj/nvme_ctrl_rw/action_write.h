#ifndef _ACTION_WRITE_H_
#define _ACTION_WRITE_H_
#include "action_base.h"
class ActionWrite : public ActionBase
{
public:
    bool is_help(void);
    bool is_valid(void);
    int issue(std::string path);
    void help_action_example(void);
    ActionWrite(int argc, char* argv[]);
    virtual ~ActionWrite(void);
protected:
private:
    int __len;
    int __addr;
    union{
    uint32_t __val32;
    uint16_t __val16;
    uint8_t __val8;
    };
    enum{
        ACTION_WRITE_ADDR=0,
        ACTION_WRITE_VAL,
        ACTION_WRITE_LEN,
        ACTION_WRITE_HELP
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _ACTION_WRITE_H_
