#include "action_reset.h"
#include "fun_action.h"
#include <fstream>
#include "utils.h"
//---------------------------------------------------------------------------
GlobalVariable ActionReset::_gVars[]=
{
    GlobalVariable("-help", "show this page\n"
                   ),
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int
ActionReset::issue(std::string path)
{
    int sc=0;
    //int pci_fd;
	
    path+="/reset_controller";
    printf("path = %s\n", path.c_str());

    std::fstream *fs;
    fs = new std::fstream(path.c_str(), std::fstream::out);
    if(fs != NULL)
    {
        char dummy=1;
        fs->write(&dummy, 1);
        fs->flush();
        delete fs;		
    }
    else
    {
        fprintf(stdout, "[%s] can not be open\n", path.c_str());
        sc=-1;
    }
    
    return sc;
}
//---------------------------------------------------------------------------
bool
ActionReset::is_help(void)
{
    return _gVars[ACTION_RESET_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
ActionReset::is_valid(void)
{
    bool valid=true;
    
    /*
    if(valid)
    {
        if((__len!=1)&&(__len!=2)&&(__len!=4)&&(__len!=8))
        {
            printf("[%s] argument with invalid value %d\n", _gVars[ACTION_READ_LEN].keyWord.c_str(), __len);
            valid=false;
        }

        if(valid)
        {
            printf("valid Action Read\n");
        }
    }*/
    return valid;
}
//---------------------------------------------------------------------------
void
ActionReset::help_action_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -act=%s -dev=1\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. reset nvme device 1\n\n");
}
//---------------------------------------------------------------------------
ActionReset::ActionReset(int argc, char* argv[])
    : ActionBase(argc, argv, FunAction::cmd_string(FunAction::FA_RESET), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
ActionReset::~ActionReset(void)
{
}
//---------------------------------------------------------------------------
