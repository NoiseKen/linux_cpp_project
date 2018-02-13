#ifndef _ASYNC_RW_HANDLER_H_
#define _ASYNC_RW_HANDLER_H_
#include "async_cmd.h"
#include "property.h"
class AsyncRWHandler : public AsyncCmd
{
public:
    virtual bool is_valid(void);
    void *rw_buffer(void);
    void assign_disk_fd(int fd);
    //use when rw buffer assign by external
    void assign_block_info(uint64_t offset, uint32_t byteCount, char *buffer);
    void dump_buffer(std::string &out);
    //rw buffer assign by external
    AsyncRWHandler(void);
    //rw buffer new by class
    AsyncRWHandler(uint64_t offset, uint32_t byteCount);
    ~AsyncRWHandler(void);
protected:
private:
    bool const __newByClass;
    char *__rwBuffer;
};

#endif //ndef _ASYNC_RW_HANDLER_H_
