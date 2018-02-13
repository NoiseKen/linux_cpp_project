#ifndef _FILE_OPERATION_H_
#define _FILE_OPERATION_H_
#include "console_arguments.h"
class FileOperation
{
public:
    enum{
        FOP_READ=0,
        FOP_WRITE,
        //FOP_WRITE_PATTERN,
        //FOP_READ_COMPARE_PATTERN,
    };
    static std::string cmd_string(int idx);
    static void list_supported_cmd(void);
    std::string const fopCmd;
    int exec(int argc, char* argv[], uint8_t nthDev);
    bool is_valid(void);
    FileOperation(std::string cmd);
    ~FileOperation(void);
protected:
private:
    static GlobalVariable _supportedPT[];
    static int _supportedPTCount;
};
#endif //ndef _GLOBAL_VARIABLE_H_
