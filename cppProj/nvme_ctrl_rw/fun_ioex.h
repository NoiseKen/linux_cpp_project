#ifndef _FUN_IOEX_H_
#define _FUN_IOEX_H_
#include "console_arguments.h"
//#include "pt_cmd.h"
class FunIoEx
{
public:
    enum{
        IOEX_DUMMY_READ=0,
        IOEX_FILL,
        IOEX_COMPARE,
    };
    static std::string cmd_string(int idx);
    static void list_supported_cmd(void);
    std::string const ioExCmd;
    int exec(int argc, char* argv[], uint8_t nthDev, std::string dd="");
    bool is_valid(void);
    FunIoEx(std::string cmd);
    ~FunIoEx(void);
protected:
private:
    static GlobalVariable _supportedAct[];
    static int _supportedActCount;
};
#endif //ndef _FUN_ACTION_H_
