#ifndef __DISK_IOEX_H__
#define __DISK_IOEX_H__
#include <string>
#include <sys/ioctl.h>
#include "disk_io.h"
#include "report_channel.h"

class IoExPlan
{
public:
    static bool directIO;
    static uint32_t backgroundPattern;
    static uint32_t nlbByteSize;
    uint16_t const tag;
    uint32_t nlb;      //issue nlb
    uint64_t slba;   //issue slba
    uint64_t rnlb;   //plan remainder nlb
    AsyncRWHandler *rwHandler;
    char *memory(void){return __buffer[(__ppflag)?0:1];}
    char *pre_memory(void){return __buffer[(__ppflag)?1:0];}
    void switch_plan_resource(void);
    bool activated_plan(void){return __starting;}
    void set_activated(void){__starting=true;}
    IoExPlan(uint32_t allocSize, uint32_t id, bool pingpong);
    ~IoExPlan(void);
private:
    char *__buffer[2];
    bool __ppmode;
    bool __starting;
    bool __ppflag;
};

class DiskIoEx : public DiskIO
{
public:
    enum{
        DISK_IOEX_SUCCESS=0,
        NOT_SUPPORT_YET=(1<<16),
        RESOURCE_INVALID,
        TASK_CANCEL,
        NOT_EXPECTED_RESULT,
        DATA_MIS_COMPARE,
        TIME_STAMP_MIS_MATCH,
        TIME_STAMP_NOT_FOUND,
        SCSI_STATUS_GOOD=(1<<17)
    };
    static std::string decode_sc(int sc);
    ReportChannel<std::string> rep;
    uint32_t run_time(void){return __duration;};
    int get_block_size(void);
    int get_disk_blocks(size_t &blocks);
    virtual void issue(void){};    
    int routine(void);
	DiskIoEx(std::string node);
	virtual ~DiskIoEx(void);
protected:
    virtual bool _run(void){return false;};
    int _progress;
    int _statusCode;
private:
    int __lastProgress;
    uint32_t __duration;
    
};


#endif //ndef __DISK_IOEX_H__
