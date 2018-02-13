#include "nvme_ioex_seq.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
IoExPlan *
NVMeIoExSeq::__new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id)
{
    //seq ioex alway use single buffer
    return new IoExPlan(allocSize, id, false);
}
//---------------------------------------------------------------------------
int
NVMeIoExSeq::caculate_progress(void)
{
    int lprogress;
    lprogress = ((__totalNlb-__remainedNlb)*100/__totalNlb);
    return lprogress;
}
//---------------------------------------------------------------------------
bool
NVMeIoExSeq::next_plan(IoExPlan *plan)
{
    bool active=false;
    uint32_t nlbPerBuf=allocBufSize/IoExPlan::nlbByteSize;
    //plan->nlb mean last time execute nlb, ex: slba=0, nlb = 4, mean fill 4 nlb from lba 0 eg lba [0~3]
    //so this time should from slba=4 (slba+nlb)
    plan->slba = __currentLBA;
    //remainder nlb, ex: 0~32 ==> nlb should 32-0+1=33

    if(__remainedNlb)
    {//not finish need active again
        //rise active flag
        active = true;
        //get io memory buffer
        //decide execute nlb
        plan->nlb = (__remainedNlb>nlbPerBuf)?nlbPerBuf:__remainedNlb;
        //reallocate new RW Handler
        //delete plan->rwHandler;
        //plan->rwHandler = new DiskRWHandler();
        plan->switch_plan_resource();        

        __currentLBA+=plan->nlb;
        __remainedNlb-=plan->nlb;
    }
    else
    {
        plan->nlb=0;
    }
    return active;
}
//---------------------------------------------------------------------------
NVMeIoExSeq::NVMeIoExSeq(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth
    , uint64_t timeStamp, std::string log)
    : NVMeIoEx(nthDev, nsid, slba, nlb, qDepth, timeStamp, log), __totalNlb(nlb)
{
    __currentLBA=slba;
    __remainedNlb=nlb;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
NVMeIoExSeq::NVMeIoExSeq(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth
    , uint64_t timeStamp, std::string log)
    : NVMeIoEx(devPath, slba, nlb, qDepth, timeStamp, log), __totalNlb(nlb)
{
    __currentLBA=slba;
    __remainedNlb=nlb;
}
//---------------------------------------------------------------------------
NVMeIoExSeq::~NVMeIoExSeq(void)
{
}
//---------------------------------------------------------------------------
