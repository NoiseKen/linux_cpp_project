#include "file_operation.h"
#include "disk_ioex.h"
#include "nvme.h"
#include "fop_read.h"
#include "fop_write.h"
//#include "fop_write_pattern.h"
//#include "fop_rc_pattern.h"
//---------------------------------------------------------------------------
GlobalVariable FileOperation::_supportedPT[]={
    GlobalVariable("read", "    Linux write command"
                   ,""),
    GlobalVariable("write", "   Linux read command"
                   ,""),
/*
    GlobalVariable("wr-known", "Write Known pattern by linux write command"
                   ,""),
    GlobalVariable("rc-known", "Read/ Compare Known pattern by linux read command"
                   ,""),
*/
    GlobalVariable("geometry", "Get Disk Geometry"
                   ,""),
    GlobalVariable("help", "    show this page\n"
                   ),
};

int FileOperation::_supportedPTCount=sizeof(_supportedPT)/sizeof(GlobalVariable);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string
FileOperation::cmd_string(int idx)
{
    std::string cmdString;
    if(idx<(_supportedPTCount-1))
    {
        cmdString=_supportedPT[idx].keyWord;
    }
    return cmdString;
}
//---------------------------------------------------------------------------
int
FileOperation::exec(int argc, char* argv[], uint8_t nthDev)
{
    int sc=-1;
    
    FOPCmd *fop=NULL;
    
    if(_supportedPT[FOP_READ].keyWord==fopCmd)
    {
        fop = new FOPCmdRead(argc, argv);
    }
    else
    if(_supportedPT[FOP_WRITE].keyWord==fopCmd)
    {
        fop = new FOPCmdWrite(argc, argv);
    }    
    else
    /*
    if(_supportedPT[FOP_WRITE_PATTERN].keyWord==fopCmd)
    {
        fop = new FOPCmdWritePattern(argc, argv);
    }
    else
    if(_supportedPT[FOP_READ_COMPARE_PATTERN].keyWord==fopCmd)
    {
        fop = new FOPCmdReadComparePattern(argc, argv);
    }
    else*/
    if(std::string("help")==fopCmd)
    {
        list_supported_cmd();
    }


    if(NULL!=fop)
    {
        if(fop->is_help())
        {
            std::string line="Arguments list";
            fop->show_var_help(line);
            fop->help_fop_example();
        }
        else
        {
            if(fop->is_valid())
            {
                sc = fop->issue(nthDev);
                printf("FOP exec : sc = %s\n", DiskIoEx::decode_sc(sc).c_str());
                if(NVME_SC_SUCCESS==sc)
                {
                    fop->post();
                }
            }
            else
            {
                printf("Invalid Argument(s)!!\n");
                sc = -EINVAL;
            }
        }
        delete fop;
    }
    
    return sc;
}
//---------------------------------------------------------------------------
bool
FileOperation::is_valid(void)
{
    bool valid=false;
    for(int i=0;i<_supportedPTCount;i++)
    {
        if(_supportedPT[i].keyWord==fopCmd)
        {
            valid = true;
            i=_supportedPTCount;
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
FileOperation::list_supported_cmd(void)
{
    ConsoleArguments *carg;
    carg = new ConsoleArguments(0, NULL, _supportedPT, sizeof(_supportedPT));
    printf("supported File Operation cmd count=%d\n", _supportedPTCount-1);
    carg->show_var_help("---------- Supported File Operation command list ----------");
    delete carg;

    printf(" ********** first time use FOP function ? ********** \n");
    printf(" %s -fop=xxx -help\n", ConsoleArguments::appName.c_str());
    printf("    ie. list FOP command xxx valid argument(s)\n\n");
    printf(" %s -fop=%s -help\n", ConsoleArguments::appName.c_str(), cmd_string(FOP_READ).c_str());
    printf("    ie. list FOP command %s valid argument(s)\n\n", cmd_string(FOP_READ).c_str());
}
//---------------------------------------------------------------------------
FileOperation::FileOperation(std::string cmd) : fopCmd(cmd)
{
}
//---------------------------------------------------------------------------
FileOperation::~FileOperation(void)
{
}
//---------------------------------------------------------------------------
