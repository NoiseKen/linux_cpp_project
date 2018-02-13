#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "disk_io.h"
#include "legacy_string.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int 
DiskIO::io_write(uint64_t offset, void *buf, size_t byteCount)
{
    int sc;
    sc=lseek(this->_id, offset, SEEK_SET);
    if(-1!=sc)
    {
        sc=write(this->_id, buf, byteCount);
    }

    if(-1==sc)
    {
        sc=-errno;
    }
    return sc;
}
//---------------------------------------------------------------------------
int 
DiskIO::io_read(uint64_t offset, void *buf, size_t byteCount)
{
    int sc;
    sc=lseek(this->_id, offset, SEEK_SET);
    if(-1!=sc)
    {
        sc=read(this->_id, buf, byteCount);
    }

    if(-1==sc)
    {
        sc=-errno;
    }
    return sc;
}
//---------------------------------------------------------------------------
int 
DiskIO::io_write(AsyncRWHandler *rwHandler)
{
    int sc=-1;
    if(rwHandler->is_valid())
    {
        rwHandler->assign_disk_fd(this->_id);
        sc = aio_write(rwHandler->command_block());
        if(sc!=0)
        {
            sc=-errno;
        }
    }    
    return sc;     
}
//---------------------------------------------------------------------------
int
DiskIO::io_read(AsyncRWHandler *rwHandler)
{
    int sc=-1;
    if(rwHandler->is_valid())
    {
        rwHandler->assign_disk_fd(this->_id);
        sc = aio_read(rwHandler->command_block());
        if(sc!=0)
        {
            sc=-errno;
        }
    }    
    return sc; 
}
//---------------------------------------------------------------------------
int 

DiskIO::io_cancel(AsyncRWHandler *rwHandler)
{
    int sc=-1;
    if(rwHandler->is_valid())
    {
        //rwHandler->assign_disk_fd(this->_id);
        sc = aio_cancel(this->_id, rwHandler->command_block());
        if(sc==-1)
        {
            sc=-errno;
        }
    }    
    return sc; 
}
//---------------------------------------------------------------------------
DiskIO::DiskIO(std::string node, bool direct) : IOCtrl(node, O_RDWR|((direct)?O_DIRECT:0))
    , directIO(direct)
{
}
//---------------------------------------------------------------------------
DiskIO::~DiskIO(void)
{

}
//---------------------------------------------------------------------------
