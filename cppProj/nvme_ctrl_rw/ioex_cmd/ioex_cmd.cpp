#include "ioex_cmd.h"
//#include "file_record.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int
IoExCmd::_task_routine(NVMeIoEx *task)
{
    std::string rep;
    int sc;
    
    sc=task->routine();

    uint32_t time;
    float dataSize;
    time=task->run_time();
    //if(time>0&&task->statusCode==NVME_SC_SUCCESS)
    if(sc==NVME_SC_SUCCESS)
    {
        dataSize=task->totalNlb*IoExPlan::nlbByteSize;
        printf("Performance : %.2f(MB/Sec)\n", (dataSize*1000/1024/1024)/time);
    }

    //final message check
    task->rep >> rep;
    if(!rep.empty())
    {
        printf("%s", rep.c_str());
    }
    //fflush(stdout);
    //int sc=task->statusCode;
    if(sc!=NVME_SC_SUCCESS)
    {
        _lastFailLba=task->last_fail_lba();
    }

    return sc;
}
//---------------------------------------------------------------------------
#if 0
void
IoExCmd::valid_io_in_file_name(int idx, bool &valid, bool acceptEmpty)
{

    AnsiString xferFileName;
    bool exist;
    xferFileName = var_to_filename(idx, exist);

    if(!acceptEmpty&&xferFileName.IsEmpty())
    {
        printf("file argument can not be empty!!\n");
        valid=false;
        return;
    }

    if(!exist&&!xferFileName.IsEmpty())
    {
        AnsiString msg;
        FileRecord *fr;
        fr = new FileRecord(xferFileName);
        exist = fr->create(true, msg);
        if(!exist)
        {//can NOT create read back file
            printf("Xfer In file cannot be created!!\n");
            printf("%s\n", msg.c_str());
            valid=false;
        }
        else
        {//if folder hierarchy is valid, remove file just created, avoid this vu command unsuccessful
            fr->remove();
        }
        delete fr;
    }
}
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
IoExCmd::IoExCmd(int argc, char* argv[], std::string cmdStr, GlobalVariable *const pVars, int varsSize)
    : ConsoleArguments(argc, argv, pVars, varsSize), cmdString(cmdStr)
    , lastFailLba(this, &IoExCmd::__f_get_last_fail_lba)
{
    _lastFailLba=0xffffffffffffffff;
}
//---------------------------------------------------------------------------
IoExCmd::~IoExCmd(void)
{
}
//---------------------------------------------------------------------------
