#include "nvme_seq_fill_lba.h"
#include "legacy_string.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
NVMeSeqFillLba::new_plan_before_issue(IoExPlan *plan)
{
    //if(!plan->activated_plan())
    {//sequential can not use ping-pong mechanism
        uint32_t *bufPtr;
        int pattern;
        int head=4, tail=(IoExPlan::nlbByteSize/sizeof(uint32_t))-1;

        //get io memory buffer
        bufPtr = (uint32_t *)plan->memory();

        //fill io memory buffer
        for(uint32_t n=0;n<plan->nlb;n++)
        {
            pattern = plan->slba+n;
            bufPtr[head]=bufPtr[head+1]=pattern;    //only fill 0x010 & 0x014
            bufPtr[tail]=bufPtr[tail-1]=pattern;    //only fill 0x1f8 & 0x1fc
            bufPtr+=IoExPlan::nlbByteSize/sizeof(uint32_t);
        }
    }
    return true; //true for write
}
//---------------------------------------------------------------------------
//void
//NVMeSeqFillLba::io_ex_post(void)
//{
//}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeSeqFillLba::NVMeSeqFillLba(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoExSeq(nthDev, nsid, slba, nlb, qDepth, timeStamp, "")
{
    rep << LegacyString().sprintf("Sequential Fill Disk LBA : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeSeqFillLba::NVMeSeqFillLba(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth, uint64_t timeStamp)
    : NVMeIoExSeq(devPath, slba, nlb, qDepth, timeStamp, "")
{
    rep << LegacyString().sprintf("Sequential Fill Disk LBA : slba=0x%lX, nlb=0x%lX\n", slba, nlb);
}
//---------------------------------------------------------------------------
NVMeSeqFillLba::~NVMeSeqFillLba(void)
{
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
