#include "nvme_task_fill_const.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void
NVMeTaskFillConst::pre_fill_after_issue(IoExPlan *plan)
{
    //predict next plan
    uint64_t rnlb=plan->rnlb-plan->nlb;
    if(rnlb)
    {
        uint32_t nlbPerBuf=allocBufSize/IoExPlan::nlbByteSize;
        uint64_t nlb=(rnlb>nlbPerBuf)?nlbPerBuf:rnlb;
        uint32_t *bufPtr;
        int pattern=dataPattern;
        int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

        //get io memory buffer
        bufPtr = (uint32_t *)plan->pre_memory();

        //fill io memory buffer
        for(uint32_t n=0;n<nlb;n++)
        {
            bufPtr[head]=bufPtr[head+1]=pattern;    //only fill 0x010 & 0x014
            bufPtr[tail]=bufPtr[tail-1]=pattern;    //only fill 0x1f8 & 0x1fc
            bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
        }
    }
}
//---------------------------------------------------------------------------
bool
NVMeTaskFillConst::new_plan_before_issue(IoExPlan *plan)
{
    if(!plan->activated_plan())
    {
        uint32_t *bufPtr;
        int pattern=dataPattern;
        int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

        //get io memory buffer
        bufPtr = (uint32_t *)plan->memory();

        //fill io memory buffer
        for(uint32_t n=0;n<plan->nlb;n++)
        {
            bufPtr[head]=bufPtr[head+1]=pattern;    //only fill 0x010 & 0x014
            bufPtr[tail]=bufPtr[tail-1]=pattern;    //only fill 0x1f8 & 0x1fc
            bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
        }
    }
    return true; //true for write
}
//---------------------------------------------------------------------------
//void
//NVMeTaskFillConst::io_ex_post(void)
//{
//}
//---------------------------------------------------------------------------
NVMeTaskFillConst::NVMeTaskFillConst(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
    , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoEx(nthDev, nsid, slba, nlb, qDepth, timeStamp, "")
    , dataPattern(pattern)
{
    rep << LegacyString().sprintf("Fill Disk Constant : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeTaskFillConst::NVMeTaskFillConst(std::string devPath, uint64_t slba, uint64_t nlb
    , uint32_t pattern, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoEx(devPath, slba, nlb, qDepth, timeStamp, "")
    , dataPattern(pattern)
{
    rep << LegacyString().sprintf("Fill Disk Constant : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeTaskFillConst::~NVMeTaskFillConst(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
