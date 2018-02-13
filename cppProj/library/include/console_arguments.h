#ifndef _CONSOLE_ARGUMENTS_H_
#define _CONSOLE_ARGUMENTS_H_
#include "global_variable.h"

class ConsoleArguments
{
public:
#define REGISTER(var)   var, sizeof(var)
    static std::string appRoot;
    static std::string appName;

    GlobalVariable *const vars;
    int const varsCount;
    void show_var_help(std::string title);
    void dump_vars(void);
    ConsoleArguments(int argc, char* argv[], GlobalVariable *const pVars, int varsSize);
    virtual ~ConsoleArguments(void);
protected:
    uint64_t var_to_int64(int idx, bool &valid);
    uint32_t var_to_int32(int idx, bool &valid);
    std::string var_to_filename(int idx, bool &exist);
    std::string valid_io_in_file_name(int idx, bool &valid, bool acceptEmpty=true);
    std::string valid_io_out_file_name(int idx, uint32_t requiredByteCount, bool &valid);
private:
    void _argv_parsing(int argc, char* argv[]);
};
#endif //ndef _GLOBAL_VARIABLE_H_
