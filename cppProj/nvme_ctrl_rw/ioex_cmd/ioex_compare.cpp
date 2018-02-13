#include "ioex_compare.h"
#include "fun_ioex.h"
#include "nvme_task_rc_const.h"
#include "nvme_task_rc_decrease.h"
#include "nvme_task_rc_increase.h"
#include "nvme_task_rc_lba.h"
#include "nvme_seq_rc_const.h"
#include "nvme_seq_rc_decrease.h"
#include "nvme_seq_rc_increase.h"
#include "nvme_seq_rc_lba.h"
//---------------------------------------------------------------------------
GlobalVariable IoExCompare::_gVars[]=
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
                   "  -seq           :  sequential compare\n"
                   ),
    GlobalVariable("-background", "background pattern fill for all LBA\n"
                   "  -background=0xffffffff : HEX mode\n"
                   "  -background=123        : DEC mode\n"
                   ,"0xffffffff" ),
    GlobalVariable("-type", "pattern type, valid value list in below\n"
                   "  -type=const : constant pattern\n"
                   "  -type=inc : increment pattern\n"
                   "  -type=dec : decrement pattern\n"
                   "  -type=lba : lba pattern\n"
                   ,"lba" ),
    GlobalVariable("-pattern", "basic data pattern, only lba type will ignore this parameter\n"
                   "  -pattern=0x1a2b3c4d : HEX mode\n"
                   "  -pattern=123        : DEC mode\n"
                   ,"0x1a2b3c4d" ),
    GlobalVariable("-interval", "interval of each dword\n"
                   "  -interval=0x1a2b3c4d : HEX mode\n"
                   "  -interval=1          : DEC mode\n"
                   ,"1" ),
    GlobalVariable("-timestamp", "time stamp for this write pattern command\n"
                   "  -timestamp=0x1234567890abcd : HEX mode\n"
                   "  -timestamp=123              : DEC mode\n"
                   ,"0" ),
    GlobalVariable("-log", "output log file when compare fail\n"
                   "  -log=rc                : actual=./rc[*1].actual\n"
                   "                           expect=./rc[*1].expect\n"
                   "  -log=/home/user/rc.bin : actual=/home/user/rc.bin[*1].actual\n"
                   "                           expect=/home/user/rc.bin[*1].expect\n"
                   "  ==> *1 be the fail lba in HEX\n"
                   ,""),
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
IoExCompare::issue(uint8_t nthDev)
{
    int sc=0;

    NVMeIoEx *task=NULL;
    _nlb++;
    if(_gVars[IOEX_CMP_SEQ].enabled)
    {
        if(_gVars[IOEX_CMP_RAW_DEV].enabled)
        {//raw path
            switch(_type)
            {
                case typeConst:
                    task=new NVMeSeqReadCompareConst(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _qd, _timeStamp, _log);
                    break;
                case typeInc:
                    task=new NVMeSeqReadCompareIncrease(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeDec:
                    task=new NVMeSeqReadCompareDecrease(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeLba:
                    task=new NVMeSeqReadCompareLba(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _qd, _timeStamp, _log);
                    break;
            }
        }
        else
        {
            switch(_type)
            {
                case typeConst:
                    task=new NVMeSeqReadCompareConst(nthDev, _nsid, _slba, _nlb, _pattern, _qd, _timeStamp, _log);
                    break;
                case typeInc:
                    task=new NVMeSeqReadCompareIncrease(nthDev, _nsid, _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeDec:
                    task=new NVMeSeqReadCompareDecrease(nthDev, _nsid, _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeLba:
                    task=new NVMeSeqReadCompareLba(nthDev, _nsid, _slba, _nlb, _qd, _timeStamp, _log);
                    break;
            }
        }
    }
    else
    {
        if(_gVars[IOEX_CMP_RAW_DEV].enabled)
        {//raw path
            switch(_type)
            {
                case typeConst:
                    task=new NVMeTaskReadCompareConst(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _qd, _timeStamp, _log);
                    break;
                case typeInc:
                    task=new NVMeTaskReadCompareIncrease(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeDec:
                    task=new NVMeTaskReadCompareDecrease(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeLba:
                    task=new NVMeTaskReadCompareLba(_gVars[IOEX_CMP_RAW_DEV].value(), _slba, _nlb, _qd, _timeStamp, _log);
                    break;
            }
        }
        else
        {
            switch(_type)
            {
                case typeConst:
                    task=new NVMeTaskReadCompareConst(nthDev, _nsid, _slba, _nlb, _pattern, _qd, _timeStamp, _log);
                    break;
                case typeInc:
                    task=new NVMeTaskReadCompareIncrease(nthDev, _nsid, _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeDec:
                    task=new NVMeTaskReadCompareDecrease(nthDev, _nsid, _slba, _nlb, _pattern, _interval, _qd, _timeStamp, _log);
                    break;
                case typeLba:
                    task=new NVMeTaskReadCompareLba(nthDev, _nsid, _slba, _nlb, _qd, _timeStamp, _log);
                    break;
            }
        }
    }
    sc=_task_routine(task);
    delete task;
 
    return sc;
}
//---------------------------------------------------------------------------
bool
IoExCompare::is_help(void)
{
    return _gVars[IOEX_CMP_HELP].enabled;
}
//---------------------------------------------------------------------------
bool
IoExCompare::is_valid(void)
{
    uint32_t background, allocBlock;
    bool valid=true;

    std::string type;
    _slba=var_to_int64(IOEX_CMP_SLBA, valid);
    _nlb=var_to_int64(IOEX_CMP_NLB, valid);
    _nsid=var_to_int32(IOEX_CMP_NSID, valid);
    _pattern=var_to_int32(IOEX_CMP_PATTERN, valid);
    _interval=var_to_int32(IOEX_CMP_INTERVAL, valid);
    _timeStamp=var_to_int64(IOEX_CMP_TIME_STAMP, valid);
    _qd = var_to_int32(IOEX_CMP_QD, valid);
    _log=valid_io_in_file_name(IOEX_CMP_LOG, valid, true);
    
    background = var_to_int32(IOEX_CMP_BACK_GROUND, valid);
    allocBlock = var_to_int32(IOEX_CMP_EXBUF, valid);    

    if(valid)
    {
        if(_qd>500)
        {
            printf("[%s] argument can not exceed 500!!\n", _gVars[IOEX_CMP_QD].keyWord.c_str());
            valid=false;
        }
        else
        if(_qd==0)
        {
            printf("[%s] argument can not be 0!!\n", _gVars[IOEX_CMP_QD].keyWord.c_str());
            valid=false;
        }
        else
        if(allocBlock==0)
        {
            printf("[%s] argument can not be 0!!\n", _gVars[IOEX_CMP_EXBUF].keyWord.c_str());
            valid=false;
        }
        else
        if(allocBlock>2048)
        {
            printf("[%s] argument can not exceed 2048!!\n", _gVars[IOEX_CMP_EXBUF].keyWord.c_str());
            valid=false;
        }
        else
        {
            type=_gVars[IOEX_CMP_PATTERN_TYPE].value();
            if(type=="const")
            {
                _type=typeConst;
            }
            else
            if(type=="inc")
            {
                _type=typeInc;
            }
            else
            if(type=="dec")
            {
                _type=typeDec;
            }
            else
            if(type=="lba")
            {
                _type=typeLba;
            }
            else
            {
                printf("Invalid data type assigned [%s]!!\n", type.c_str());
                valid=false;
            }
        }

        if(valid)
        {
            printf("valid IO Extension command\n");
            NVMeIoEx::allocBufSize=allocBlock*512;
            IoExPlan::backgroundPattern=background;
        }
    }
    return valid;
}
//---------------------------------------------------------------------------
void
IoExCompare::help_ioex_example(void)
{
    printf(" ********** %s example **********\n", cmdString.c_str());
    printf(" %s -ioex=%s -nlb=0x7ffff -qd=32\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. read/ compare LBA pattern 0x80000 sector data from slba 0, using SW QD=32, extension buffer 1M\n\n");
    printf(" %s -ioex=%s -slba=123 -nlb=0x7ffff -qd=4 -buf-blk=8 -type=inc\n", ConsoleArguments::appName.c_str(), cmdString.c_str());
    printf("    ie. read/ compare increment pattern 0x80000 sector data from slba 123, using SW QD=4, extension buffer 4K\n\n");
}
//---------------------------------------------------------------------------
IoExCompare::IoExCompare(int argc, char* argv[])
    : IoExCmd(argc, argv, FunIoEx::cmd_string(FunIoEx::IOEX_COMPARE), REGISTER(_gVars))
{
}
//---------------------------------------------------------------------------
IoExCompare::~IoExCompare(void)
{
}
//---------------------------------------------------------------------------
