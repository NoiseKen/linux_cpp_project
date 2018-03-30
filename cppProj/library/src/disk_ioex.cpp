//#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <linux/fs.h>
#include "disk_ioex.h"
#include "legacy_string.h"
#include "utils.h"

//---------------------------------------------------------------------------
bool IoExPlan::directIO=false;
uint32_t IoExPlan::backgroundPattern=0xffffffff;
uint32_t IoExPlan::nlbByteSize=512;
//---------------------------------------------------------------------------
void
IoExPlan::switch_plan_resource(void)
{
    delete this->rwHandler;
    this->__ppflag ^= 1;
    this->rwHandler = new AsyncRWHandler();
//line.sprintf("RC Buffer=0x%x", plan->memory());
}
//---------------------------------------------------------------------------
IoExPlan::IoExPlan(uint32_t allocSize, uint32_t id, bool pingpong)
    : tag(id), __ppmode(pingpong)
{
    rwHandler = new AsyncRWHandler();
    __ppflag=false;
    if(allocSize)
    {
        uint32_t *ptr32, cnt;
        //__buffer[0]=new char[allocSize];
        //__buffer[1]=(__ppmode)?new char[allocSize]:__buffer[0];
        if(IoExPlan::directIO)
        {
            __buffer[0]=(char *)memalign(IoExPlan::nlbByteSize, allocSize);
            __buffer[1]=(__ppmode)?(char *)memalign(IoExPlan::nlbByteSize, allocSize):__buffer[0];
        }
        else
        {
            __buffer[0]=(char *)malloc(allocSize);
            __buffer[1]=(__ppmode)?(char *)malloc(allocSize):__buffer[0];
        }

        ptr32=(uint32_t *)__buffer[0];
        cnt=allocSize/sizeof(uint32_t);
        while(cnt--)
        {
            *ptr32=backgroundPattern;
            ptr32++;
        }

        if(__ppmode)
        {
            ptr32=(uint32_t *)__buffer[1];
            cnt=allocSize/sizeof(uint32_t);
            while(cnt--)
            {
                *ptr32=backgroundPattern;
                ptr32++;
            }
        }
    }
    else
    {
        __buffer[0]=NULL;
        __buffer[1]=NULL;
    }
    __starting=false;
}
//---------------------------------------------------------------------------
IoExPlan::~IoExPlan(void)
{
    delete rwHandler;
    if(NULL != __buffer[0])
    {
        //delete __buffer[0];
        free(__buffer[0]);
    }

    if(NULL != __buffer[1])
    {
        if(__ppmode)
        {
            //delete __buffer[1];
            free(__buffer[1]);
        }
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int 
DiskIoEx::get_disk_blocks(size_t &blocks)
{
    int sc;
    sc = this->pass_through(BLKGETSIZE, &blocks);
    //printf("blockSize=%ld, sc=%d\n", blockSize, sc);
    if(0!=sc)
    {
        printf("Get Disk Blocks fail [%s]!!\n", this->decode_sc(sc).c_str());
    }

    return sc;
}
//---------------------------------------------------------------------------
int 
DiskIoEx::get_block_size(void)
{
//#dfine BLKGETSIZE64 _IOR(0x12,114,size_t)
    int sc;
    size_t blockSize=0;
    sc = this->pass_through(BLKSSZGET, &blockSize);
    //sc = this->pass_through(BLKGETSIZE64, &blockSize);
    //printf("blockSize=%ld, sc=%d\n", blockSize, sc);
    if(0!=sc)
    {
        printf("Get Block Size fail [%s]!!\n", this->decode_sc(sc).c_str());
    }
    else
    {
        sc=blockSize;
    }
    return sc;
}
//---------------------------------------------------------------------------
std::string
DiskIoEx::decode_sc(int sc)
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
            CASE_TO_STR(DISK_IOEX_SUCCESS);
            CASE_TO_STR(NOT_SUPPORT_YET);
            CASE_TO_STR(RESOURCE_INVALID);
            CASE_TO_STR(TASK_CANCEL);
            CASE_TO_STR(NOT_EXPECTED_RESULT);                        
            CASE_TO_STR(DATA_MIS_COMPARE);
            CASE_TO_STR(TIME_STAMP_MIS_MATCH);
            CASE_TO_STR(TIME_STAMP_NOT_FOUND);
            default:
                desc = lstr.sprintf("Undefined");
                break;
        }
    }
    return desc;
#undef CASE_TO_STR  //(n)   case n:desc = #n;break
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int 
DiskIoEx::routine(void)
{
    //some condition in construct found task cannot be progress
    //here to force terminate it, set statusCode non zero can be done
    if(_statusCode==0)
    {
        __duration=getticks();
#define PROGRESS_GAP    10
        std::string msg;
        issue();        
        while(_run())
        {
            this->rep >> msg;
            if(!msg.empty())
            {
                printf("%s", msg.c_str());
            }
            else
            {
                if(_progress > (__lastProgress+PROGRESS_GAP))
                {
                    LegacyString line;
                    __lastProgress=_progress;
                    printf("%s\n", line.sprintf("PROGRESS : %3d%%", __lastProgress).c_str());
                    fflush(stdout);
                }
            }
        };
        __duration=getticks()-__duration;

        this->rep >> msg;
        if(!msg.empty())
        {
            printf("%s", msg.c_str());
        }

#undef PROGRESS_GAP   // 10
    }
    
    return _statusCode;
}
//---------------------------------------------------------------------------
DiskIoEx::DiskIoEx(std::string node) : DiskIO(node, IoExPlan::directIO)
{
    int blk = this->get_block_size();
    if(blk>0)
    {
        IoExPlan::nlbByteSize=(uint32_t)blk;
        _statusCode = 0;
    }
    else
    {
        printf("Unexpected nlb size can not retrieved from ioctl!!\n");
        _statusCode=-1;
    }

    size_t blocks=0;
    if(this->get_disk_blocks(blocks)==0)
    {
        printf("Disk Geometry : blocks/block size/total size=[0x%lx/ 0x%x/ %.2fG]\n", blocks, blk, (blocks*blk)/(1024*1024*1024.0));
    }
    else
    {
        printf("Unexpected disk blocks can not retrieved from ioctl!!\n");
        _statusCode=-1;
    }

    _progress=0;
    __lastProgress=-1;
    __duration=0;
}
//---------------------------------------------------------------------------
DiskIoEx::~DiskIoEx(void)
{

}
//---------------------------------------------------------------------------
