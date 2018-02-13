#include <stdlib.h>
#include <errno.h>
#include "async_cmd.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int 
AsyncCmd::__f_get_status_code(void)
{
    int sc=this->__statusCode;
    if(sc!=0)
    {//for aio function, aio_error get status non zero postive value if io fail
     //make this condition for a fail status code        
        sc=-sc;
    }    
    return sc;
}
//---------------------------------------------------------------------------
struct aiocb *
AsyncCmd::command_block(void)
{
    return this->_aiocb;
}
//---------------------------------------------------------------------------
bool
AsyncCmd::is_valid(void)
{
    return (NULL!=this->_aiocb)?true:false;
}
//---------------------------------------------------------------------------
bool
AsyncCmd::cmd_finished(void)
{
    this->__statusCode=aio_error(this->_aiocb);
    return (EINPROGRESS==this->__statusCode)?false:true;
}
//---------------------------------------------------------------------------
AsyncCmd::AsyncCmd(void) : statusCode(this, &AsyncCmd::__f_get_status_code)
{
    this->_aiocb=(struct aiocb *)calloc(1, sizeof(struct aiocb));
}
//---------------------------------------------------------------------------
AsyncCmd::~AsyncCmd(void)
{
    if(NULL!=this->_aiocb)
    {
        free(this->_aiocb);
    }
}
//---------------------------------------------------------------------------
