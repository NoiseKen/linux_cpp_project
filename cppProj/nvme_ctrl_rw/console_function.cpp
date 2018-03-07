#include <stdio.h>
#include "console_function.h"
#include "sys_ctrl.h"
#include "file_ctrl.h"
//---------------------------------------------------------------------------
GlobalVariable ConsoleFunction::_gVars[]=
{
    GlobalVariable("-act", "action function\n"
                   "  -act=help   : list all valid action\n"
                   ,""),
    GlobalVariable("-ioex", "io extension function\n"
                   "  -act=help   : list all valid ioex\n"
                   ,""),
    GlobalVariable("-fop", "file operation function\n"
                   "  -fop=help  : list all valid file operation command\n"
                   ,""),
    GlobalVariable("-dev", "target nvme device number\n"
                   "  -dev=0     : select nvme device [0] in this system \n"
                   ,"0"),
    GlobalVariable("-list", "nvme device list\n"
                   "  -list      : list all present NVMeDevice(s)\n"
                   ),
    GlobalVariable("-ver", "Get Driver Version (confirm current Driver is KW Debug Driver)\n"
                   "  -ver       : Check Current NVMe Driver is KW Debug or not\n"
                   ),
    GlobalVariable("-direct", "Direct IO option for io extension function\n"
                   "  -direct    : Use Direct IO in ioex function\n"
                   ),
    GlobalVariable("-dd", "debug dump\n"
                   "  -dd                   : gen debug dump file to default name (./debug.dump)\n"
                   "  -dd=log/debug         : gen debug dump file to ./log/debug\n"
                   "  -dd=/home/user/dd.bin : gen debug dump file to /home/user/dd.bin\n\n\n"
                   "  *********** about Debug Dump File *********** \n"
                   "  type   : binary file\n"
                   "  format : in IoEx Action (16 bytes)\n"
                   "           [ 7: 0] : fail/ unexpected status's lba\n"
                   "           [11: 8] : status code(same value as this utility return)\n"
                   "           [15:12] : reserved\n"
                   "                     all 16 bytes 0xff, if IoEx action completion without fail\n"
                   ),
    //GlobalVariable("-rawdev", "", SW_DEFAULT, true),
    HideSwitch("-rawdev"),

};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool 
ConsoleFunction::is_direct_io(void)
{
    return _gVars[FUNC_DIRECT_IO].enabled;
}
//---------------------------------------------------------------------------
int
ConsoleFunction::selected_nvme_device(void)
{
    bool valid=true;    
    int nth;
    nth=SysCtrl::string_to_int32(_gVars[FUNC_DEV].value(), valid);

    if(!valid)
    {
        printf("%s : Invalid Parameter [%s]\n", _gVars[FUNC_DEV].keyWord.c_str(), _gVars[FUNC_DEV].value().c_str());
        printf("use default device number : 0\n");
        nth=0;
    }

    if(nth<0)
    {
        printf("A negative value[%d] change to default device number : 0\n", nth);
        nth=0;
    }

    return nth;  
}
//---------------------------------------------------------------------------
bool
ConsoleFunction::is_list_device(void)
{
    return _gVars[FUNC_LIST].enabled;
}
//---------------------------------------------------------------------------
bool 
ConsoleFunction::is_driver_version(void)
{
    return _gVars[FUNC_VER].enabled;
}
//---------------------------------------------------------------------------
bool ConsoleFunction::is_raw_device(std::string &rawdev)
{
    bool en=_gVars[FUNC_HIDE_RAWDEV].enabled;
    if(en)
    {
        rawdev=_gVars[FUNC_HIDE_RAWDEV].value();
    }
    return en;
}
//---------------------------------------------------------------------------
bool
ConsoleFunction::action_command(std::string &cmd)
{
    cmd = _gVars[FUNC_ACT].value();
    return !cmd.empty();
}
//---------------------------------------------------------------------------
bool
ConsoleFunction::ioex_command(std::string &cmd)
{
    cmd = _gVars[FUNC_IOEX].value();
    return !cmd.empty();
}
//---------------------------------------------------------------------------
bool
ConsoleFunction::fop_command(std::string &cmd)
{
    cmd = _gVars[FUNC_FOP].value();
    return !cmd.empty();
}
#if 0
//---------------------------------------------------------------------------
bool
ConsoleFunction::scsi_command(AnsiString &cmd)
{
    cmd = _gVars[FUNC_SCSI].value;
    return !cmd.IsEmpty();
}
//---------------------------------------------------------------------------
bool
ConsoleFunction::tool_function(AnsiString &cmd)
{
    cmd = _gVars[FUNC_TOOL].value;
    return !cmd.IsEmpty();
}
#endif
//---------------------------------------------------------------------------
bool
ConsoleFunction::is_valid(void)
{
    bool valid=true;
    if(   (_gVars[FUNC_ACT].value().length()==0)
        &&(_gVars[FUNC_IOEX].value().length()==0)
        &&(_gVars[FUNC_FOP].value().length()==0)
    )
    {
        if(   !_gVars[FUNC_LIST].enabled
            &&!_gVars[FUNC_VER].enabled
        )
        {
            valid=false;
        }
    }


    if(valid)
    {
        if((_gVars[FUNC_DD].enabled)&&(_gVars[FUNC_DD].value().empty()))
        {//default dd
            _gVars[FUNC_DD].set_value("debug.dump");
        }

        _debugDump=valid_io_in_file_name(FUNC_DD, valid, true);
        if(!valid)
        {
            printf("Debug Dump file can not be created!!\n");
        }

    }

    return valid;
}
//---------------------------------------------------------------------------
ConsoleFunction::ConsoleFunction(int argc, char* argv[])
    : ConsoleArguments(argc, argv, REGISTER(_gVars))
{
//    if(_gVars[2].value
}
//---------------------------------------------------------------------------
ConsoleFunction::~ConsoleFunction(void)
{
}
//---------------------------------------------------------------------------

