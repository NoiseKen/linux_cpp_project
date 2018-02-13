#ifndef __DISK_IO_H__
#define __DISK_IO_H__
#include <string>
#include <sys/ioctl.h>
#include "property.h"
#include "io_ctrl.h"
#include "async_rw_handler.h"
class DiskIO : public IOCtrl
{
public:
    bool const directIO;
    int io_read(uint64_t offset, void *buf, size_t byteCount);
    int io_write(uint64_t offset, void *buf, size_t byteCount);
    int io_read(AsyncRWHandler *rwHandler);
    int io_write(AsyncRWHandler *rwHandler);
    int io_cancel(AsyncRWHandler *rwHandler);
	DiskIO(std::string node, bool direct);
	~DiskIO(void);
protected:
private:
};


#endif //ndef __DISK_IO_H__
