#include "fun_action.h"
#include "action_read.h"
#include "action_write.h"
#include "action_reset.h"
#include "action_dump.h"
//---------------------------------------------------------------------------
GlobalVariable FunAction::_supportedAct[]={
    GlobalVariable("read", "    Read Controller Register"
                   ,""),
    GlobalVariable("write", "   Write Controller Register"
                   ,""),
    GlobalVariable("reset", "   Reset Controller"
                   ,""),
    GlobalVariable("dump", "    Dump Controller Register"
                   ,""),
    GlobalVariable("help", "    show this page\n"
                   ),
};

int FunAction::_supportedActCount=sizeof(_supportedAct)/sizeof(GlobalVariable);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string
FunAction::cmd_string(int idx)
{
    std::string cmdString;
    if(idx<(_supportedActCount-1))
    {
        cmdString=_supportedAct[idx].keyWord;
    }
    return cmdString;
}
//---------------------------------------------------------------------------
int
FunAction::exec(int argc, char* argv[], std::string path)
{
    int sc=-1;

    ActionBase *action=NULL;

    if(_supportedAct[FA_READ].keyWord==faCmd)
    {
        action = new ActionRead(argc, argv);
    }
    else
    if(_supportedAct[FA_WRITE].keyWord==faCmd)
    {
        action = new ActionWrite(argc, argv);
    }
    else
    if(_supportedAct[FA_RESET].keyWord==faCmd)
    {
        action = new ActionReset(argc, argv);
    }
    else 
    if(_supportedAct[FA_DUMP].keyWord==faCmd)
    {
        action = new ActionDump(argc, argv);
    }
    else
    if(std::string("help")==faCmd)
    {
        list_supported_cmd();
    }

    if(NULL!=action)
    {
        if(action->is_help())
        {
            std::string line="Arguments list";
            action->show_var_help(line);
            action->help_action_example();
        }
        else
        {
            if(action->is_valid())
            {
                sc = action->issue(path);
                /*
                if(!dd.IsEmpty())
                {
                    __int64 lba=ioex->lastFialLba;
                    DWORD rsvd=0xffffffff;
                    TMemoryStream *ms;
                    ms = new TMemoryStream();
                    ms->Write(&lba, sizeof(__int64));
                    ms->Write(&sc, sizeof(int));
                    ms->Write(&rsvd, sizeof(int));                    
                    ms->SaveToFile(dd);
                    delete ms;
                }


                printf("IO Extension exec : sc = %s\n", NVMeDevice::completion_decode(sc));
                */
                if(0==sc)
                {
                    action->post();
                }
            }
            else
            {
                printf("Invalid Argument(s)!!\n");
                sc = -1;
            }
        }
        delete action;
    }

    return sc;
}
//---------------------------------------------------------------------------
bool
FunAction::is_valid(void)
{
    bool valid=false;
    for(int i=0;i<_supportedActCount;i++)
    {
        if(_supportedAct[i].keyWord==faCmd)
        {
            valid = true;
            i=_supportedActCount;
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
FunAction::list_supported_cmd(void)
{
    ConsoleArguments *carg;
    carg = new ConsoleArguments(0, NULL, _supportedAct, sizeof(_supportedAct));
    printf("supported Action count=%d\n", _supportedActCount-1);
    carg->show_var_help("---------- Supported Action list ----------");
    delete carg;

    printf(" ********** first time use Action function ? ********** \n");
    printf(" %s -act=xxx -help\n", ConsoleArguments::appName.c_str());
    printf("    ie. list Action xxx valid argument(s)\n\n");
    printf(" %s -act=%s -help\n", ConsoleArguments::appName.c_str(), cmd_string(FA_READ).c_str());
    printf("    ie. list Action %s valid argument(s)\n\n", cmd_string(FA_READ).c_str());
}
//---------------------------------------------------------------------------
FunAction::FunAction(std::string cmd) : faCmd(cmd)
{
}
//---------------------------------------------------------------------------
FunAction::~FunAction(void)
{
}
//---------------------------------------------------------------------------
