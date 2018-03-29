#include "fun_ioex.h"
#include "ioex_dummy_read.h"
#include "ioex_fill.h"
#include "ioex_compare.h"
#include "memory.h"
//---------------------------------------------------------------------------
GlobalVariable FunIoEx::_supportedAct[]={
    GlobalVariable("dummy-read", "IoEx Dummy Read"
                   ,""),
    GlobalVariable("fill", "      IoEx Fill"
                   ,""),
    GlobalVariable("compare", "   IoEx Compare"
                   ,""),
    GlobalVariable("help", "      show this page\n"
                   ),
};

int FunIoEx::_supportedActCount=sizeof(_supportedAct)/sizeof(GlobalVariable);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string
FunIoEx::cmd_string(int idx)
{
    std::string cmdString;
    if(idx<(_supportedActCount-1))
    {
        cmdString=_supportedAct[idx].keyWord;
    }
    return cmdString;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int
FunIoEx::exec(int argc, char* argv[], uint8_t nthDev, std::string dd)
{
    int sc=-1;

    IoExCmd *ioex=NULL;

    if(_supportedAct[IOEX_DUMMY_READ].keyWord==ioExCmd)
    {
       ioex = new IoExDummyRead(argc, argv);
    }
    else
    if(_supportedAct[IOEX_FILL].keyWord==ioExCmd)
    {
        ioex = new IoExFill(argc, argv);
    }
    else
    if(_supportedAct[IOEX_COMPARE].keyWord==ioExCmd)
    {
        ioex = new IoExCompare(argc, argv);
    }
    else
    if(std::string("help")==ioExCmd)
    {
        list_supported_cmd();
    }

    if(NULL!=ioex)
    {
        if(ioex->is_help())
        {
            std::string line="Arguments list";
            ioex->show_var_help(line);
            ioex->help_ioex_example();
        }
        else
        {
            if(ioex->is_valid())
            {
                sc = ioex->issue(nthDev);
                if(!dd.empty())
                {                    
                    //uint64_t lba=ioex->lastFailLba;
                    //uint32_t rsvd=0xffffffff;
                    Memory<uint32_t> *memDebug;
                    memDebug=new Memory<uint32_t>(16/sizeof(uint32_t));
                    union{
                    uint64_t *lba;
                    uint32_t *dw32;
                    };
                    lba=(uint64_t *)memDebug->memory();
                    *lba++=ioex->lastFailLba;
                    *dw32++=sc;
                    *dw32++=0xffffffff;
                    
                    int sc;
                    sc=memDebug->save_to(dd, 16);
                    printf("dd [%s] save : %d\n", dd.c_str(), sc);
                    //ms->Write(&lba, sizeof(__int64));
                    //ms->Write(&sc, sizeof(int));
                    //ms->Write(&rsvd, sizeof(int));                    
                    //ms->SaveToFile(dd);
                    delete memDebug;
                }

                printf("IO Extension exec : sc = %s\n", NVMeIoEx::decode_sc(sc).c_str());
                if(NVME_SC_SUCCESS==sc)
                {
                    ioex->post();
                }
            }
            else
            {
                printf("Invalid Argument(s)!!\n");
                sc = -EINVAL;
            }
        }
        delete ioex;
    }

    return sc;
}
//---------------------------------------------------------------------------
bool
FunIoEx::is_valid(void)
{
    bool valid=false;
    for(int i=0;i<_supportedActCount;i++)
    {
        if(_supportedAct[i].keyWord==ioExCmd)
        {
            valid = true;
            i=_supportedActCount;
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
FunIoEx::list_supported_cmd(void)
{
    ConsoleArguments *carg;
    carg = new ConsoleArguments(0, NULL, _supportedAct, sizeof(_supportedAct));
    printf("supported Io Extension command count=%d\n", _supportedActCount-1);
    carg->show_var_help("---------- Supported Io Extension list ----------");
    delete carg;

    printf(" ********** first time use IoEx function ? ********** \n");
    printf(" %s -ioex=xxx -help\n", ConsoleArguments::appName.c_str());
    printf("    ie. list Io Extension xxx valid argument(s)\n\n");
    printf(" %s -ioex=%s -help\n", ConsoleArguments::appName.c_str(), cmd_string(IOEX_DUMMY_READ).c_str());
    printf("    ie. list Io Extension %s valid argument(s)\n\n", cmd_string(IOEX_DUMMY_READ).c_str());
}
//---------------------------------------------------------------------------
FunIoEx::FunIoEx(std::string cmd) : ioExCmd(cmd)
{
}
//---------------------------------------------------------------------------
FunIoEx::~FunIoEx(void)
{
}
//---------------------------------------------------------------------------
