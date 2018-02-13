#include "nvme_task_fill_increase.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
NVMeTaskFillIncrease::pre_fill_after_issue(IoExPlan *plan)
{
    //predict next plan
    uint64_t rnlb=plan->rnlb-plan->nlb;
    if(rnlb)
    {
        uint32_t nlbPerBuf=allocBufSize/IoExPlan::nlbByteSize;
        uint64_t slba=plan->slba+plan->nlb;
        uint64_t nlb=(rnlb>nlbPerBuf)?nlbPerBuf:rnlb;
        uint32_t *bufPtr;
        int pattern=dataPattern;
        int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

        //get io memory buffer
        bufPtr = (uint32_t *)plan->pre_memory();

        //fill io memory buffer
        for(uint32_t n=0;n<nlb;n++)
        {
            pattern=dataPattern+(head+((slba+n)*IoExPlan::nlbByteSize/sizeof(uint32_t)))*dataInterval;
            bufPtr[head]=pattern;                   //only fill 0x010 & 0x014
            bufPtr[head+1]=pattern+dataInterval;
            pattern=dataPattern+(tail+((slba+n)*IoExPlan::nlbByteSize/sizeof(uint32_t)))*dataInterval;
            bufPtr[tail]=pattern;       //only fill 0x1f8 & 0x1fc
            bufPtr[tail-1]=pattern-dataInterval;
            bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
        }
    }
}
//---------------------------------------------------------------------------
bool
NVMeTaskFillIncrease::new_plan_before_issue(IoExPlan *plan)
{
    if(!plan->activated_plan())
    {
        uint32_t *bufPtr;
        int pattern;
        int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

        //get io memory buffer
        bufPtr = (uint32_t *)plan->memory();

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
    return true; //true for write
}
//---------------------------------------------------------------------------
//void
//NVMeTaskFillIncrease::io_ex_post(void)
//{
//}
//---------------------------------------------------------------------------
NVMeTaskFillIncrease::NVMeTaskFillIncrease(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
    , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoEx(nthDev, nsid, slba, nlb, qDepth, timeStamp, "")
    , baseLba(slba), dataInterval(interval), dataPattern(pattern)
{
    rep << LegacyString().sprintf("Fill Disk Increase : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeTaskFillIncrease::NVMeTaskFillIncrease(std::string devPath, uint64_t slba, uint64_t nlb
    , uint32_t pattern, uint32_t interval, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoEx(devPath, slba, nlb, qDepth, timeStamp, "")
    , baseLba(slba), dataInterval(interval), dataPattern(pattern)
{
    rep << LegacyString().sprintf("Fill Disk Increase : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeTaskFillIncrease::~NVMeTaskFillIncrease(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
