#ifndef _FUN_ACTION_H_
#define _FUN_ACTION_H_
#include "console_arguments.h"
//#include "pt_cmd.h"
class FunAction
{
public:
    enum{
        FA_READ=0,
        FA_WRITE,
        FA_RESET,
        FA_DUMP,
    };
    static std::string cmd_string(int idx);
    static void list_supported_cmd(void);
    std::string const faCmd;
    int exec(int argc, char* argv[], std::string path);
    bool is_valid(void);
    FunAction(std::string cmd);
    ~FunAction(void);
protected:
private:
    static GlobalVariable _supportedAct[];
    static int _supportedActCount;
};
#endif //ndef _FUN_ACTION_H_
