#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "memory.h"
#include "nvme_ioex.h"
#include "legacy_string.h"
#include "nvme_status.h"
//---------------------------------------------------------------------------
uint32_t NVMeIoEx::allocBufSize=1024*1024;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string 
NVMeIoEx::decode_sc(int sc)
{
#define CASE_TO_STR(n)      case n:desc = #n;break
    std::string desc;
    LegacyString lstr;
    if(sc<0)
    {//system failure
        desc=lstr.sprintf("%d, %s", sc, strerror(-sc));
    }
    else
    {//nomal status code
        switch(sc)
        {
            CASE_TO_STR(NVME_SC_SUCCESS);
            CASE_TO_STR(NVME_SC_INVALID_OPCODE);
            CASE_TO_STR(NVME_SC_INVALID_FIELD);
            CASE_TO_STR(NVME_SC_CMDID_CONFLICT);
            CASE_TO_STR(NVME_SC_DATA_XFER_ERROR);
            CASE_TO_STR(NVME_SC_POWER_LOSS);
            CASE_TO_STR(NVME_SC_INTERNAL);
            CASE_TO_STR(NVME_SC_ABORT_REQ);
            CASE_TO_STR(NVME_SC_ABORT_QUEUE);
            CASE_TO_STR(NVME_SC_FUSED_FAIL);
            CASE_TO_STR(NVME_SC_FUSED_MISSING);
            CASE_TO_STR(NVME_SC_INVALID_NS);
            CASE_TO_STR(NVME_SC_CMD_SEQ_ERROR);
            CASE_TO_STR(NVME_SC_SGL_INVALID_LAST);
            CASE_TO_STR(NVME_SC_SGL_INVALID_COUNT);
            CASE_TO_STR(NVME_SC_SGL_INVALID_DATA);
            CASE_TO_STR(NVME_SC_SGL_INVALID_METADATA);
            CASE_TO_STR(NVME_SC_SGL_INVALID_TYPE);

            CASE_TO_STR(NVME_SC_SGL_INVALID_OFFSET);
            CASE_TO_STR(NVME_SC_SGL_INVALID_SUBTYPE);

            CASE_TO_STR(NVME_SC_LBA_RANGE);
            CASE_TO_STR(NVME_SC_CAP_EXCEEDED);
            CASE_TO_STR(NVME_SC_NS_NOT_READY);
            CASE_TO_STR(NVME_SC_RESERVATION_CONFLICT);

            /*
             * Command Specific Status:
             */
            CASE_TO_STR(NVME_SC_CQ_INVALID);
            CASE_TO_STR(NVME_SC_QID_INVALID);
            CASE_TO_STR(NVME_SC_QUEUE_SIZE);
            CASE_TO_STR(NVME_SC_ABORT_LIMIT);
            CASE_TO_STR(NVME_SC_ABORT_MISSING);
            CASE_TO_STR(NVME_SC_ASYNC_LIMIT);
            CASE_TO_STR(NVME_SC_FIRMWARE_SLOT);
            CASE_TO_STR(NVME_SC_FIRMWARE_IMAGE);
            CASE_TO_STR(NVME_SC_INVALID_VECTOR);
            CASE_TO_STR(NVME_SC_INVALID_LOG_PAGE);
            CASE_TO_STR(NVME_SC_INVALID_FORMAT);
            CASE_TO_STR(NVME_SC_FW_NEEDS_CONV_RESET);
            CASE_TO_STR(NVME_SC_INVALID_QUEUE);
            CASE_TO_STR(NVME_SC_FEATURE_NOT_SAVEABLE);
            CASE_TO_STR(NVME_SC_FEATURE_NOT_CHANGEABLE);
            CASE_TO_STR(NVME_SC_FEATURE_NOT_PER_NS);
            CASE_TO_STR(NVME_SC_FW_NEEDS_SUBSYS_RESET);
            CASE_TO_STR(NVME_SC_FW_NEEDS_RESET);
            CASE_TO_STR(NVME_SC_FW_NEEDS_MAX_TIME);
            CASE_TO_STR(NVME_SC_FW_ACIVATE_PROHIBITED);
            CASE_TO_STR(NVME_SC_OVERLAPPING_RANGE);
            CASE_TO_STR(NVME_SC_NS_INSUFFICENT_CAP);
            CASE_TO_STR(NVME_SC_NS_ID_UNAVAILABLE);
            CASE_TO_STR(NVME_SC_NS_ALREADY_ATTACHED);
            CASE_TO_STR(NVME_SC_NS_IS_PRIVATE);
            CASE_TO_STR(NVME_SC_NS_NOT_ATTACHED);
            CASE_TO_STR(NVME_SC_THIN_PROV_NOT_SUPP);
            CASE_TO_STR(NVME_SC_CTRL_LIST_INVALID);

            /*
             * I/O Command Set Specific - NVM commands:
             */
            CASE_TO_STR(NVME_SC_BAD_ATTRIBUTES);
            CASE_TO_STR(NVME_SC_INVALID_PI);
            CASE_TO_STR(NVME_SC_READ_ONLY);

            #if 0
            /*
             * I/O Command Set Specific - Fabrics commands:
             */
            CASE_TO_STR(NVME_SC_CONNECT_FORMAT);
            CASE_TO_STR(NVME_SC_CONNECT_CTRL_BUSY);
            CASE_TO_STR(NVME_SC_CONNECT_INVALID_PARAM);
            CASE_TO_STR(NVME_SC_CONNECT_RESTART_DISC);
            CASE_TO_STR(NVME_SC_CONNECT_INVALID_HOST);

            CASE_TO_STR(NVME_SC_DISCOVERY_RESTART);
            CASE_TO_STR(NVME_SC_AUTH_REQUIRED);
            #endif

            /*
             * Media and Data Integrity Errors:
             */
            CASE_TO_STR(NVME_SC_WRITE_FAULT);
            CASE_TO_STR(NVME_SC_READ_ERROR);
            CASE_TO_STR(NVME_SC_GUARD_CHECK);
            CASE_TO_STR(NVME_SC_APPTAG_CHECK);
            CASE_TO_STR(NVME_SC_REFTAG_CHECK);
            CASE_TO_STR(NVME_SC_COMPARE_FAILED);
            CASE_TO_STR(NVME_SC_ACCESS_DENIED);
            CASE_TO_STR(NVME_SC_UNWRITTEN_BLOCK);

            /*
             * General status code from DiskIoEx
             */
            CASE_TO_STR(NOT_SUPPORT_YET);
            CASE_TO_STR(RESOURCE_INVALID);
            CASE_TO_STR(TASK_CANCEL);
            CASE_TO_STR(NOT_EXPECTED_RESULT);
            CASE_TO_STR(DATA_MIS_COMPARE);
            CASE_TO_STR(TIME_STAMP_MIS_MATCH);
            CASE_TO_STR(TIME_STAMP_NOT_FOUND);
            default:
                desc = lstr.sprintf("0x%04X (sc=0x%02X, sct=0x%02X)", sc, (sc>>0)&0xFF, (sc>>8)&0x07);
                break;
        }
    }
    return desc;
#undef CASE_TO_STR  //(n)   case n:desc = #n;break
}
//---------------------------------------------------------------------------
void 
NVMeIoEx::_save_dmc_log(int *expect, int *actual, uint64_t lba, IoExPlan *plan)
{
    LegacyString expectFN, actualFN, total;
    uint64_t size;
    if(__consoleLog.empty())
    {
        rep << "No configured DMC fail log, skip it!!\n";
        return;
    }

    get_time_stamp_header(lba, (TimeStampHeader*)expect);

    expectFN.sprintf("%s[0x%lx].expect", __consoleLog.c_str(), lba);
    actualFN.sprintf("%s[0x%lx].actual", __consoleLog.c_str(), lba);
    total.sprintf("%s[0x%lx].total", __consoleLog.c_str(), plan->slba);

    Memory<int> *memExpect, *memActual, *memTotal;
    memExpect=new Memory<int>(expect);
    memActual=new Memory<int>(actual);
    memTotal=new Memory<int>((int *)plan->memory());
    size=memExpect->save_to(expectFN.str(), IoExPlan::nlbByteSize);
    rep << expectFN.sprintf("Save expect pattern to file [%s] -- %ld\n", expectFN.str().c_str(), size);
    size=memActual->save_to(actualFN.str(), IoExPlan::nlbByteSize);
    rep << actualFN.sprintf("Save actual pattern to file [%s] -- %ld\n", actualFN.str().c_str(), size);
    size=memTotal->save_to(total.str(), plan->nlb*IoExPlan::nlbByteSize);
    rep << total.sprintf("Save  total pattern to file [%s] -- %ld\n", total.str().c_str(), size);
    
    
    /*
    printf("expect:0x%lx, actaul=0x%lx\n", (uint64_t)expect, (uint64_t)actual);
    std::string out;
    memExpect->dump(0, 512, out);
    printf("Expect:\n%s", out.c_str());
    memActual->dump(0, 512, out);
    printf("Actual:\n%s", out.c_str());
    */
    
    delete memExpect;
    delete memActual;
    delete memTotal;
}
//---------------------------------------------------------------------------
bool 
NVMeIoEx::get_time_stamp_header(uint64_t slba, TimeStampHeader *hdr)
{
    bool good=true;
    if(0==__timeStampConsle)
    {//if not time stamp keep background
        uint32_t *hdrConsole=(uint32_t *)hdr;
        for(int i=0;i<4;i++)
        {
            hdrConsole[i]=IoExPlan::backgroundPattern;
        }
    }
    else
    {//insert time stamp header
        hdr->slba=slba;
        hdr->timeStamp=__timeStampConsle;
    }
    return good;
}
//---------------------------------------------------------------------------
int
NVMeIoEx::check_time_stamp(IoExPlan *plan, uint32_t &nlbOffset)
{
    LegacyString line;
    int sc=0;
    TimeStampHeader *expect;
    TimeStampHeader *actual;
    expect = new TimeStampHeader();
    actual = (TimeStampHeader *)plan->memory();

    for(uint32_t i=0;i<plan->nlb;i++)
    {
        bool match=false;
        //__int64 slbaExpected=TimeStampLog::translate_slba(expected);        
        get_time_stamp_header(plan->slba+i, expect);
        if(actual->slba==expect->slba)
        {
            if(actual->timeStamp==expect->timeStamp)
            {
                match=true;
            }
        }

        if(match)
        {
            actual += IoExPlan::nlbByteSize/sizeof(TimeStampHeader);
        }
        else
        {//time stamp not match
            line.sprintf("FAIL : time stamp mismatch in lba 0x%lX\n", plan->slba+i);
            rep << line.str();

            line.sprintf("SLBA, expected=0x%lX, actual=0x%lX\n", expect->slba, actual->slba);
            rep << line.str();
            line.sprintf("Time Stamp, expected=0x%lX, actual=0x%lX\n"
                , expect->timeStamp, actual->timeStamp);
            rep << line.str();

            sc = DiskIoEx::TIME_STAMP_MIS_MATCH;
            nlbOffset = i;
            break;
        }

    }

    delete expect;
    return sc;
}
//---------------------------------------------------------------------------
void 
NVMeIoEx::issue(void)
{
    _io_plan_scheduler(testSlba, totalNlb, testQDepth, __planList);
}
//---------------------------------------------------------------------------
void 
NVMeIoEx::cancel(void)
{
    //int sc=0;
    for(int i=0;i<ioPlanCount;i++)
    {
        IoExPlan *plan;
        plan = ioPlan[i];
        //sc=this->io_cancel(plan->rwHandler);
        this->io_cancel(plan->rwHandler);
        //LegacyString line;
        //rep << line.sprintf("Cancel IO[%d] -- %d [%s]\n", plan->tag, sc, NVMeIoEx::decode_sc(sc).c_str());
    }
}
//---------------------------------------------------------------------------
bool
NVMeIoEx::next_plan(IoExPlan *plan)
{///
    bool active=false;
    uint32_t nlbPerBuf=allocBufSize/IoExPlan::nlbByteSize;
    //plan->nlb mean last time execute nlb, ex: slba=0, nlb = 4, mean fill 4 nlb from lba 0 eg lba [0~3]
    //so this time should from slba=4 (slba+nlb)
    plan->slba += plan->nlb;
    //remainder nlb, ex: 0~32 ==> nlb should 32-0+1=33
    plan->rnlb -= plan->nlb;
    if(plan->rnlb)
    {//not finish need active again
        //rise active flag
        active = true;
        //get io memory buffer
        //decide execute nlb
        plan->nlb = (plan->rnlb>nlbPerBuf)?nlbPerBuf:plan->rnlb;
        //reallocate new RW Handler
        //delete plan->rwHandler;
        //plan->rwHandler = new DiskRWHandler();
        plan->switch_plan_resource();
    }
    else
    {
        plan->nlb=0;
    }

    return active;
}
//---------------------------------------------------------------------------
int 
NVMeIoEx::good_status_after_plan_completion(IoExPlan *plan, uint32_t &errOffset)
{
    return NVME_SC_SUCCESS;
}
//---------------------------------------------------------------------------
bool 
NVMeIoEx::check_plan_completion(IoExPlan *plan, int &sc, uint32_t &errOffset)
{
    bool done=true;
    if(plan->activated_plan())
    {
        done = plan->rwHandler->cmd_finished();
        if(done)
        {
            sc = plan->rwHandler->statusCode;
            if((NVME_SC_SUCCESS==sc)&&(plan->nlb!=0))
            {
                sc = good_status_after_plan_completion(plan, errOffset);
            }
        }
    }
    else
    {
        sc=NVME_SC_SUCCESS;
    }
    return done;
}
//---------------------------------------------------------------------------
bool
NVMeIoEx::_run(void)
{
    int sc;
    bool stop=true;
    bool update=false;
    uint32_t errOffset=0;
    for(int i=0;i<ioPlanCount;i++)
    {
        IoExPlan *plan;
        plan = ioPlan[i];
        if(check_plan_completion(plan, sc, errOffset))
        {
            //AnsiString line;
            //line.sprintf("IO Extend[%d]", i);
            //NVMeDevice::completion_decode(sc, line);

            if(0==sc)
            {
                bool planActivated=plan->activated_plan();
                if(next_plan(plan))
                {//there is next plan exist
                    stop=false;
                    update=true;
                    bool write = new_plan_before_issue(plan);
                    if(write)
                    {
                        if(true) //timeStampSwitch)
                        {//Add time stamp
                            uint32_t incUnit=IoExPlan::nlbByteSize/sizeof(uint64_t);
                            uint64_t *hdrConsole;

                            hdrConsole = (uint64_t *)plan->memory();

                            for(uint32_t i=0;i<plan->nlb;i++)
                            {
                                //if time stamp not give, keep background, else insert time stamp header
                                if(0!=__timeStampConsle)
                                {
                                    hdrConsole[0]=plan->slba+i;
                                    hdrConsole[1]=__timeStampConsle;
                                }
                                hdrConsole+=incUnit;
                            }
                        }
                        
                        plan->rwHandler->assign_block_info(plan->slba*IoExPlan::nlbByteSize, plan->nlb*IoExPlan::nlbByteSize, plan->memory());
                        //_nvmeDev->write(plan->slba, plan->nlb, plan->memory(), plan->rwHandler);
                        this->io_write(plan->rwHandler);
                        pre_fill_after_issue(plan);
                    }
                    else
                    {
                        plan->rwHandler->assign_block_info(plan->slba*IoExPlan::nlbByteSize, plan->nlb*IoExPlan::nlbByteSize, plan->memory());
                        this->io_read(plan->rwHandler);
                        //_nvmeDev->read(plan->slba, plan->nlb, plan->memory(), plan->rwHandler);
                    }

                    if(!planActivated)
                    {
                        plan->set_activated();
                    }
                }
                else
                {
                    //LegacyString line;
                    //line.sprintf("Q[%d] : all done", plan->tag);
                    //printf("%s\n", line.str().c_str());
                }
            }
            else
            {//not expected io error or nvme completion status
                LegacyString line;
                __failLBA = plan->slba+errOffset;
                line.sprintf("slba=0x%016lx, nlb=0x%08X -- sc = %s\n", plan->slba ,plan->nlb, NVMeIoEx::decode_sc(sc).c_str());
                rep << line.str();
                cancel();
                _statusCode = sc;
                stop = true;
                break;
            }
        }
        else
        {   //at least one task still running
            //cnt++;
            stop = false;
        }
        //remainderNlb += plan->rnlb;
    }

    //if(stop)
    //{//all done or not expected error
    //    Terminate();
    //}

    if(update)
    {//calculate progress
        int lprogress;
        lprogress = caculate_progress();
        //lprogress = ((totalNlb-remainderNlb)*100/totalNlb);
        if(lprogress!=_progress)
        {
            _progress = lprogress;
        }
    }
    
    return !stop;
}
//---------------------------------------------------------------------------
int
NVMeIoEx::caculate_progress(void)
{
    uint64_t remainderNlb=0;
    for(int i=0;i<ioPlanCount;i++)
    {
        IoExPlan *plan;
        plan = ioPlan[i];
        remainderNlb += plan->rnlb;
    }

    int lprogress;
    lprogress = ((totalNlb-remainderNlb)*100/totalNlb);
    return lprogress;
}
//---------------------------------------------------------------------------
IoExPlan *
NVMeIoEx::__new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id)
{
    return new IoExPlan(allocSize, id, true);
}
//---------------------------------------------------------------------------
void 
NVMeIoEx::_io_plan_scheduler(uint64_t slba, uint64_t nlb, uint32_t qDepth, std::list<IoExPlan *> *planList)
{
    uint32_t oneTimeNlb;
    uint32_t buffer=allocBufSize;
    uint64_t split;

    if(0!=nlb)
    {
        //1024*1024/512 = 2048 ==> one buffer can consum 2048 nlb
//if(nvmeAttr->lbaSize==0)
//{
//line.sprintf("%s : ASSERT LBA Size=0", __FUNC__ );
//memoDebug->msg_box(line);
//}
        oneTimeNlb = buffer/IoExPlan::nlbByteSize;
        //if use default buffer size, and xfer one time, how many split needed
//if(oneTimeNlb==0)
//{
//line.sprintf("%s : ASSERT oneTimeNlb=0, allocBufSize=%d", __FUNC__, buffer );
//memoDebug->msg_box(line);
//}
        split = (nlb+oneTimeNlb-1)/oneTimeNlb;
        //if needed split small than assign qDepth, use split
        qDepth = (qDepth>split)?split:qDepth;
        //split change to nlb for one qDepth
        //exp nlb = 100, qDepth = 3 ==> split = 100/3 = 33, if slba = 0
        //Q0 [0~32] ==> 33 lnlb, Q1[33~65] ==> 33 lnlb, Q3[66~99] ==> 34 nlb
//if(qDepth==0)
//{
//line.sprintf("%s : ASSERT qDepth=0", __FUNC__ );
//memoDebug->msg_box(line);
//}
        split = nlb/qDepth;
        IoExPlan *plan=NULL;
        for(uint32_t i=0;i<qDepth;i++)
        {//assign init plan list
            plan = __new_ioex_plan_in_scheduler(buffer, i);//new IoExPlan(buffer, i);
            plan->slba = slba;
            plan->rnlb = split;
            plan->nlb = 0;
            //point to next thread slba
            slba += split;
            nlb-=split;
            planList->push_back(plan);
        }

        //fix last plan elba
        if((0!=nlb) && (plan!=NULL))
        {
            plan->rnlb+=nlb;
        }
    }
}
//---------------------------------------------------------------------------
IoExPlan *
NVMeIoEx::__f_get_ioex_plan(int i)
{
    std::list<IoExPlan *>::iterator it=__planList->begin();
    std::advance(it, i);
    return *it;
}
//---------------------------------------------------------------------------
NVMeIoEx::NVMeIoEx(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth
    , uint64_t timeStamp, std::string log) : DiskIoEx(LegacyString().sprintf("/dev/nvme%dn%d", nthDev, nsid))
    , testSlba(slba), totalNlb(nlb), testQDepth(qDepth)
    , ioPlanCount(this, &NVMeIoEx::__f_get_plan_count), ioPlan(this, &NVMeIoEx::__f_get_ioex_plan)
    , __consoleLog(log), __timeStampConsle(timeStamp)
{
    __planList=new std::list<IoExPlan *>();
    /*
    int blk = this->get_block_size();
    if(blk>0)
    {
        IoExPlan::nlbByteSize=(uint32_t)blk;
    }
    else
    {
        rep << "Unexpected nlb size can not retrieved from ioctl!!\n";
        _statusCode=-1;
    }
    */
}
//---------------------------------------------------------------------------
NVMeIoEx::NVMeIoEx(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth
    , uint64_t timeStamp, std::string log) : DiskIoEx(devPath)
    , testSlba(slba), totalNlb(nlb), testQDepth(qDepth)
    , ioPlanCount(this, &NVMeIoEx::__f_get_plan_count), ioPlan(this, &NVMeIoEx::__f_get_ioex_plan)
    , __consoleLog(log), __timeStampConsle(timeStamp)
{
    __planList=new std::list<IoExPlan *>();
    /*
    int blk = this->get_block_size();
    if(blk>0)
    {
        IoExPlan::nlbByteSize=(uint32_t)blk;
    }
    else
    {
        rep << "Unexpected nlb size can not retrieved from ioctl!!\n";
        _statusCode=-1;
    }
    */
}
//---------------------------------------------------------------------------
NVMeIoEx::~NVMeIoEx(void)
{
    int loop=0;
    for (std::list<IoExPlan *>::iterator it = __planList->begin(); it != __planList->end(); it++)
    {
        loop++;
        delete *it;
    }
    //printf("delete loop=%d\n", loop);
    delete __planList;

}
//---------------------------------------------------------------------------
