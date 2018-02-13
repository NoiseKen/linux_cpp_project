#include <unistd.h>
//#include <cstdio>
//#include <iostream>
//#include <memory>
#include <stdexcept>
#include <string>
#include "sys_ctrl.h"

std::string 
SysCtrl::string_rtrim(std::string str, std::string trimStr)
{
    if(str.empty())
    {
        return str;
    }
    
    str.erase(str.find_last_not_of(trimStr)+1);
    return str;
}

int
SysCtrl::string_to_int32(std::string str, bool &valid)
{
    uint32_t to=0;
    std::string head="";
    //valid=true;
    size_t pos=0;
    try{
        if(str.length()>2)
        {
            head=std::string(str.c_str(), 2);
        }            

        if(head=="0x")
        {
            str.substr(3, str.length()-2);
            to = std::stoul(str, &pos, 16);
        }
        else
        {
            to = std::stoul(str, &pos, 10);
        }

        if(pos!=str.length())
        {
            throw std::invalid_argument("Invalid integer value");
        }
    }
    catch(const std::invalid_argument& ia)
    {
        valid=false;
    }
    return to;
}


uint64_t
SysCtrl::string_to_int64(std::string str, bool &valid)
{
    uint64_t to=0;
    std::string head="";
    //valid=true;
    size_t pos=0;
    try{
        if(str.length()>2)
        {
            head=std::string(str.c_str(), 2);
        }            

        if(head=="0x")
        {
            str.substr(3, str.length()-2);
            to = std::stoull(str, &pos, 16);
        }
        else
        {
            to = std::stoull(str, &pos, 10);
        }

        if(pos!=str.length())
        {
            throw std::invalid_argument("Invalid integer value");
        }
    }
    catch(const std::invalid_argument& ia)
    {
        valid=false;
    }
    return to;
}

int 
SysCtrl::exec_cmd(std::string cmd)
{
    int ret;
    char buffer[64];
    _stdout="";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, sizeof(buffer), pipe) != NULL)
                _stdout += buffer;
        }
    } catch (...) {
        ret=pclose(pipe);
        throw;
    }
    ret=pclose(pipe);
    return ret;
}

SysCtrl::SysCtrl(void)
{
}

SysCtrl::~SysCtrl(void)
{
}
