#ifndef __IO_CTRL_H__
#define __IO_CTRL_H__
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "property.h"

class IOCtrl
{
public:
	//operator
	//method
	int pass_through(unsigned long cmdTag, void *cbw);
	//useful properties
	property_ro<IOCtrl, bool>bConnected;
    property_ro<IOCtrl, int>errorCode;
	//constructor
	//IOCtrl(int);
	IOCtrl(std::string node, int flag=O_RDWR);
	//destructor	
	~IOCtrl(void);
protected:
	int _id;
private:
    int __f_get_error_code(void);
	bool __f_get_connected(void);

};


#endif //ndef __IO_CTRL_H__
