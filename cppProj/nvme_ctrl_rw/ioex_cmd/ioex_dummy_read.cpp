#include "ioex_dummy_read.h"
#include "fun_ioex.h"
#include "nvme_task_dummy_read.h"
#include "nvme_seq_dummy_read.h"
//---------------------------------------------------------------------------
GlobalVariable IoExDummyRead::_gVars[]=
{
    GlobalVariable("-slba", "start lba\n"
                   "  -slba=0x11223344  : HEX mode\n"
                   "  -slba=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-nlb", "number of lba\n"
                   "  -nlb=0x11223344  : HEX mode\n"
                   "  -nlb=33          : DEC mode\n"
                   ,"0"),
    GlobalVariable("-nsid", "namespace identifier (NSID)\n"
                   "  -nlb=1           : nsid=1\n"
                   ,"1"),
    GlobalVariable("-qd", "queue depth\n"
                   "  -qd=1 : 1 outstanding SW queue\n"
                   ,"1" ),
    GlobalVariable("-buf-blk", "extension buffer block size, one block = 512 bytes\n"
                   "  -buf-blk=2     :   2 block, extension buffer = 1024 bytes\n"
                   "  -buf-blk=0x100 : 256 block, extension buffer = 128K bytes\n"
                   ,"2048" ),
    GlobalVariable("-seq", "enable type option, sequential\n"
                   "  -seq           :  sequential dummy read\n"
                   ),
    GlobalVariable("-rawdev", "raw string for dev node path\n"
                   "  -rawdev=/dev/nvme1n1 : ioex function to /dev/nvme0n1 (same as -dev=1 -nsid=1)\n"
                   "  -rawdev=/dev/sda     : ioex function to /dev/sda\n"
                   ),
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
IoExDummyRead::issue(uint8_t nthDev)
{
    int sc=0;

    NVMeIoEx *task;

    if(_gVars[IOEX_DUMMY_READ_RAW_DEV].enabled)
    {//raw path
        if(_gVars[IOEX_DUMMY_READ_SEQ].enabled)
        {
            task=new NVMeSeqDummyRead(_gVars[IOEX_DUMMY_READ_RAW_DEV].value(), _slba, _nlb+1, _qd);
        }
        else
        {
            task=new NVMeTaskDummyRead(_gVars[IOEX_DUMMY_READ_RAW_DEV].value(), _slba, _nlb+1, _qd);
        }
    }
    else
    {
        if(_gVars[IOEX_DUMMY_READ_SEQ].enabled)
        {
            task=new NVMeSeqDummyRead(nthDev, _nsid, _slba, _nlb+1, _qd);
        }
        else
        {
            task=new NVMeTaskDummyRead(nthDev, _nsid, _slba, _nlb+1, _qd);
        }
    }
    sc=_task_routine(task);
    delete task;
    return sc;
}
//---------------------------------------------------------------------------
bool
IoExDummyRead::is_help(void)
{
    return _gVars[IOEX_DUMMY_READ_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
IoExDummyRead::is_valid(void)
{
    //__int64 slba, nlb;
    uint32_t allocBlock;
    bool valid=true;

    _slba=var_to_int64(IOEX_DUMMY_READ_SLBA, valid);
    _nlb=var_to_int64(IOEX_DUMMY_READ_NLB, valid);
    _nsid=var_to_int32(IOEX_DUMMY_READ_NSID, valid);
    _qd = var_to_int32(IOEX_DUMMY_READ_QD, valid);
    allocBlock = var_to_int32(IOEX_DUMMY_READ_EXBUF, valid);

    if(valid)
    {
        if(_qd>500)
        {
            printf("[%s] argument can not exceed 500!!\n", _gVars[IOEX_DUMMY_READ_QD].keyWord.c_str());
            valid=false;
        }
        else
        if(_qd==0)
        {
            printf("[%s] argument can not be 0!!\n", _gVars[IOEX_DUMMY_READ_QD].keyWord.c_str());
            valid=false;
        }
        else
        if(allocBlock==0)
        {
            printf("[%s] argument can not be 0!!\n", _gVars[IOEX_DUMMY_READ_EXBUF].keyWord.c_str());
            valid=false;
        }
        else
        if(allocBlock>2048)
        {
            printf("[%s] argument can not exceed 2048!!\n", _gVars[IOEX_DUMMY_READ_EXBUF].keyWord.c_str());
            valid=false;
        }


        if(valid)
        {
            printf("valid IO Extension command\n");
            NVMeIoEx::allocBufSize=allocBlock*512;
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
IoExDummyRead::help_ioex_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -ioex=%s -nlb=0x7ffff -qd=32\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. dummy read 0x80000 sector data from slba 0, using SW QD=32, extension buffer 1M\n\n");
    printf(" %s -ioex=%s -slba=123 -nlb=0x7ffff -qd=4 -buf-blk=8\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. dummy read 0x80000 sector data from slba 123, using SW QD=4, extension buffer 4K\n\n");
}
//---------------------------------------------------------------------------
IoExDummyRead::IoExDummyRead(int argc, char* argv[])
    : IoExCmd(argc, argv, FunIoEx::cmd_string(FunIoEx::IOEX_DUMMY_READ), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
IoExDummyRead::~IoExDummyRead(void)
{
}
//---------------------------------------------------------------------------
