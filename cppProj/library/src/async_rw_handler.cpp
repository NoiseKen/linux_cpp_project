#include "memory.h"
#include "async_rw_handler.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void *
AsyncRWHandler::rw_buffer(void)
{
    return this->__rwBuffer;
}
//---------------------------------------------------------------------------
void 
AsyncRWHandler::dump_buffer(std::string &out)
{
    Memory<int> *mem;
    mem = new Memory<int>((int *)this->__rwBuffer);
    mem->dump(0, this->_aiocb->aio_nbytes, out);
    delete mem;
}
//---------------------------------------------------------------------------
bool 
AsyncRWHandler::is_valid(void)
{
    bool valid=false;
    if(AsyncCmd::is_valid())
    {
        valid=(NULL!=this->__rwBuffer)?true:false;
    }
    return valid;
}
//---------------------------------------------------------------------------
void 
AsyncRWHandler::assign_block_info(uint64_t offset, uint32_t byteCount, char *buffer)
{
    if(this->_aiocb!=NULL)
    {
        this->__rwBuffer        =buffer;
        this->_aiocb->aio_buf   =buffer;
        this->_aiocb->aio_offset=offset;
        this->_aiocb->aio_nbytes=byteCount;
    }
}
//---------------------------------------------------------------------------
void 
AsyncRWHandler::assign_disk_fd(int fd)
{
    this->_aiocb->aio_fildes=fd;
}
//---------------------------------------------------------------------------
AsyncRWHandler::AsyncRWHandler(void) : AsyncCmd()
    , __newByClass(false)
{
}
//---------------------------------------------------------------------------
AsyncRWHandler::AsyncRWHandler(uint64_t offset, uint32_t byteCount) : AsyncCmd()
    , __newByClass(true)
{
    if(this->_aiocb!=NULL)
    {
        if(NULL!=(this->__rwBuffer   =new char[byteCount]))
        {
            //this->_aiocbp->aio_reqprio = 0;
            this->_aiocb->aio_offset=offset;
            this->_aiocb->aio_nbytes=byteCount;
            this->_aiocb->aio_buf   =this->__rwBuffer;
        }
    }
}
//---------------------------------------------------------------------------
AsyncRWHandler::~AsyncRWHandler(void)
{
    if(this->__newByClass)
    {
        if(NULL!=this->__rwBuffer)
        {
            delete this->__rwBuffer;
        }
    }
}
//---------------------------------------------------------------------------
