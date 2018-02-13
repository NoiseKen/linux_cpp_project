#include <unistd.h>
//#include "debug.h"
#include "io_ctrl.h"
//---------------------------------------------------------------------------
int 
IOCtrl::__f_get_error_code(void)
{
    return this->_id;
}
//---------------------------------------------------------------------------
bool
IOCtrl::__f_get_connected(void)
{	
	return (this->_id<0)?false:true;
}
//---------------------------------------------------------------------------
int
IOCtrl::pass_through(unsigned long cmdTag, void *cbw)
{
	int sc;
    sc=ioctl(this->_id, cmdTag, cbw);
    if(sc!=0)
    {
        sc=-errno;
    }
    return sc;
}
//---------------------------------------------------------------------------
/*
IOCtrl::IOCtrl(int fd) : bConnected(this, &IOCtrl::__f_get_connected)
{
	id = fd;
}
*/
//---------------------------------------------------------------------------
IOCtrl::IOCtrl(std::string node, int flag) : bConnected(this, &IOCtrl::__f_get_connected)
    ,errorCode(this, &IOCtrl::__f_get_error_code)
{

    this->_id = open(node.c_str(), flag);
    
    if(this->_id<0)
    {
        this->_id=-errno;
    }
}
//---------------------------------------------------------------------------
IOCtrl::~IOCtrl(void)
{
	if(this->_id>=0)
	{
		close(this->_id);
	}
}
//---------------------------------------------------------------------------
