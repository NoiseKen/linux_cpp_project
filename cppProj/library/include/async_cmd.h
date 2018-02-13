#ifndef _ASYNC_CMD_H_
#define _ASYNC_CMD_H_
#include <stdint.h>
#include <stdlib.h>
#include <aio.h>
#include "property.h"
class AsyncCmd
{
public:
    property_ro<AsyncCmd, int>statusCode;
    struct aiocb *command_block(void);
    virtual bool is_valid(void);
    bool cmd_finished(void);
    AsyncCmd(void);
    virtual ~AsyncCmd(void);
protected:
    struct aiocb *_aiocb;
private:
    int __statusCode;
    int __f_get_status_code(void);
};

#endif//ndef _ASYNC_CMD_H_
