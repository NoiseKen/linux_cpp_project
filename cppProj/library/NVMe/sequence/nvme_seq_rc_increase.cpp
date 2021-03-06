#include "nvme_seq_rc_increase.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
NVMeSeqReadCompareIncrease::golden_pattern_build(IoExPlan *plan)
{
    uint32_t *bufPtr;
    int pattern;
    int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

    //get io memory buffer
    bufPtr = (uint32_t *)golden->memory();

    //fill io memory buffer
    for(uint32_t n=0;n<plan->nlb;n++)
    {
        pattern=dataPattern+(head+((plan->slba+n)*IoExPlan::nlbByteSize/sizeof(uint32_t)))*dataInterval;
        bufPtr[head]=pattern;                   //only fill 0x010 & 0x014
        bufPtr[head+1]=pattern+dataInterval;
        pattern=dataPattern+(tail+((plan->slba+n)*IoExPlan::nlbByteSize/sizeof(uint32_t)))*dataInterval;
        bufPtr[tail]=pattern;       //only fill 0x1f8 & 0x1fc
        bufPtr[tail-1]=pattern-dataInterval;
        bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeSeqReadCompareIncrease::NVMeSeqReadCompareIncrease(uint8_t nthDev, uint8_t nsid, uint64_t slba
    , uint64_t nlb, uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
    , std::string log, std::string desc)
    : NVMeSeqReadCompare(nthDev, nsid, slba, nlb, qDepth, timeStamp, log, desc)
    , baseLba(slba), dataInterval(interval), dataPattern(pattern)
{
}
//---------------------------------------------------------------------------
NVMeSeqReadCompareIncrease::NVMeSeqReadCompareIncrease(std::string devPath, uint64_t slba
    , uint64_t nlb, uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp
    , std::string log, std::string desc)
    : NVMeSeqReadCompare(devPath, slba, nlb, qDepth, timeStamp, log, desc)
    , baseLba(slba), dataInterval(interval), dataPattern(pattern)
{
}
//---------------------------------------------------------------------------
NVMeSeqReadCompareIncrease::~NVMeSeqReadCompareIncrease(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
