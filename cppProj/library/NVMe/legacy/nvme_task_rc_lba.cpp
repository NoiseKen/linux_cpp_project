#include "nvme_task_rc_lba.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
NVMeTaskReadCompareLba::golden_pattern_build(IoExPlan *plan)
{
    uint32_t *bufPtr;
    int pattern;
    int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;
    bufPtr=(uint32_t *)golden->memory();
    for(uint32_t n=0;n<plan->nlb;n++)
    {
        pattern=plan->slba+n;
        bufPtr[head]=bufPtr[head+1]=pattern;    //only fill 0x010 & 0x014
        bufPtr[tail]=bufPtr[tail-1]=pattern;    //only fill 0x1f8 & 0x1fc
        bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeTaskReadCompareLba::NVMeTaskReadCompareLba(uint8_t nthDev, uint8_t nsid, uint64_t slba
    , uint64_t nlb, uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc)
    : NVMeTaskReadCompare(nthDev, nsid, slba, nlb, qDepth, timeStamp, log, desc)
{
}
//---------------------------------------------------------------------------
NVMeTaskReadCompareLba::NVMeTaskReadCompareLba(std::string devPath, uint64_t slba
    , uint64_t nlb, uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc)
    : NVMeTaskReadCompare(devPath, slba, nlb, qDepth, timeStamp, log, desc)
{
}
//---------------------------------------------------------------------------
NVMeTaskReadCompareLba::~NVMeTaskReadCompareLba(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
