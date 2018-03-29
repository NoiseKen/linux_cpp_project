#ifndef __NVME_IOEX_H__
#define __NVME_IOEX_H__
#include <string>
#include <list>
#include "property.h"
#include "disk_ioex.h"

typedef struct _TimeStampHeader{
    uint64_t slba;
    uint64_t timeStamp;
}TimeStampHeader;

class NVMeIoEx : public DiskIoEx
{
public:
	static uint32_t allocBufSize;
    static std::string decode_sc(int sc);
    uint64_t const testSlba;
    uint64_t const totalNlb;
    uint32_t const testQDepth;

    property_ro<NVMeIoEx, int>ioPlanCount;
    properties_ro<NVMeIoEx, IoExPlan *>ioPlan;
    int check_time_stamp(IoExPlan *plan, uint32_t &nlbOffset);
    bool get_time_stamp_header(uint64_t slba, TimeStampHeader *hdr);
    uint64_t last_fail_lba(void) {return __failLBA;};
    void issue(void);
    void cancel(void);
    bool check_plan_completion(IoExPlan *plan, int &sc, uint32_t &errOffset);
	NVMeIoEx(uint8_t nthDev, uint8_t nsid, uint64_t slba, uint64_t nlb, uint32_t qDepth
        , uint64_t timeStamp, std::string log);
	NVMeIoEx(std::string devPath, uint64_t slba, uint64_t nlb, uint32_t qDepth
        , uint64_t timeStamp, std::string log);
	virtual ~NVMeIoEx(void);
protected:
    //assign next plan, return true : next plan exist, flase : there is no next plan
    virtual bool next_plan(IoExPlan *plan);
    virtual int good_status_after_plan_completion(IoExPlan *plan, uint32_t &errOffset);
    //for write use to fill pattern to plan memory
    virtual bool new_plan_before_issue(IoExPlan *plan) {return false;};
    virtual void pre_fill_after_issue(IoExPlan *plan){};
    virtual int caculate_progress(void);
    bool _run(void);
    void _io_plan_scheduler(uint64_t slba, uint64_t nlb, uint32_t qDepth, std::list<IoExPlan *> *planList);
    void _save_dmc_log(int *expect, int *actual, uint64_t lba, IoExPlan *plan);
private:
    virtual IoExPlan* __new_ioex_plan_in_scheduler(uint32_t allocSize, uint32_t id);
    std::list<IoExPlan *> *__planList;
    int __f_get_plan_count(void) {return __planList->size();};
    IoExPlan *__f_get_ioex_plan(int i);
    std::string const __consoleLog;
    uint64_t const __timeStampConsle;
    uint64_t __failLBA;
};


#endif //ndef __DISK_IOEX_H__
