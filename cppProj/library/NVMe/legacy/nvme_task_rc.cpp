#include "nvme_task_rc.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
IoExPlan *
NVMeTaskReadCompare::__new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id)
{
    //ioex compare alway use single buffer
    return new IoExPlan(allocSize, id, false);
}
//---------------------------------------------------------------------------
int
NVMeTaskReadCompare::good_status_after_plan_completion(IoExPlan *plan, uint32_t &errOffset)
{
    bool pass=false;
    int sc=NVME_SC_SUCCESS;
    LegacyString line;
    std::string out;
    Memory<uint32_t> *read;
    read = new Memory<uint32_t>((uint32_t *)plan->memory());
    golden_pattern_build(plan);
    //add time stamp
    //DWORD errOffset=0;
    sc = check_time_stamp(plan, errOffset);
    if(NVME_SC_SUCCESS==sc)
    {
        for(uint32_t i=0;i<plan->nlb;i++)
        {
            pass = read->compare((uint32_t *)golden->memory()
                    , IoExPlan::nlbByteSize-sizeof(TimeStampHeader), out, sizeof(TimeStampHeader)+i*IoExPlan::nlbByteSize);

            if(!pass)
            {
                rep << out;
                line.sprintf("Compare fail in IO Command : slba=0x%016lx, nlb=0x%08x\n", plan->slba, plan->nlb);
                rep << line.str();
                sc = DiskIoEx::DATA_MIS_COMPARE;
                errOffset = i;
                break;
            }
        }
    }
    if((NVME_SC_SUCCESS!=sc)||!pass)
    {
        int *expect, *actual;
        expect =(int *)golden->memory();
        expect += errOffset*IoExPlan::nlbByteSize/sizeof(int);

        actual = (int *)plan->memory();
        actual += errOffset*IoExPlan::nlbByteSize/sizeof(int);
        //store log
        _save_dmc_log(expect, actual, plan->slba+errOffset, plan);
    }

    delete read;
    return sc;
}
//---------------------------------------------------------------------------
//void
//NVMeTaskReadCompare::io_ex_post(void)
//{
//}
//---------------------------------------------------------------------------
NVMeTaskReadCompare::NVMeTaskReadCompare(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb
    , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc)
    : NVMeIoEx(nthDev, nsid, slba, nlb, qDepth, timeStamp, log)
{
    rep << LegacyString().sprintf("Read/ Compare %s : slba=0x%lx, nlb=0x%lx\n", desc.c_str(), slba, nlb);
    golden = new Memory<uint32_t>(allocBufSize/sizeof(uint32_t));
    golden->fill(IoExPlan::backgroundPattern, allocBufSize);
}
//---------------------------------------------------------------------------
NVMeTaskReadCompare::NVMeTaskReadCompare(std::string devPath, uint64_t slba, uint64_t nlb
    , uint32_t qDepth, uint64_t timeStamp, std::string log, std::string desc)
    : NVMeIoEx(devPath, slba, nlb, qDepth, timeStamp, log)
{
    rep << LegacyString().sprintf("Read/ Compare %s : slba=0x%lx, nlb=0x%lx\n", desc.c_str(), slba, nlb);
    golden = new Memory<uint32_t>(allocBufSize/sizeof(uint32_t));
    golden->fill(IoExPlan::backgroundPattern, allocBufSize);
}
//---------------------------------------------------------------------------
NVMeTaskReadCompare::~NVMeTaskReadCompare(void)
{
    delete golden;
//memoDebug->msg_log(AnsiString().sprintf("%s", __FUNC__));
}
//---------------------------------------------------------------------------
