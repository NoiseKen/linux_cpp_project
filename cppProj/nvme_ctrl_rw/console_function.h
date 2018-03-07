#ifndef _CONSOLE_FUNCTION_H_
#define _CONSOLE_FUNCTION_H_
#include "console_arguments.h"
class ConsoleFunction : public ConsoleArguments
{
public:
    bool is_valid(void);
    bool action_command(std::string &cmd);
    bool ioex_command(std::string &cmd);
    bool fop_command(std::string &cmd);
    bool is_list_device(void);
    bool is_driver_version(void);
    bool is_direct_io(void);
    bool is_raw_device(std::string &rawdev);
    std::string debug_dump(void) {return _debugDump;};
    int selected_nvme_device(void);
    ConsoleFunction(int argc, char* argv[]);
    ~ConsoleFunction(void);
protected:
private:
    std::string _debugDump;
    enum{
        FUNC_ACT=0,
        FUNC_IOEX,
        FUNC_FOP,
        FUNC_DEV,
        FUNC_LIST,
        FUNC_VER,
        FUNC_DIRECT_IO,
        FUNC_DD,
        FUNC_HIDE_RAWDEV,
    };
    static GlobalVariable _gVars[];
};
#endif //ndef _GLOBAL_VARIABLE_H_
