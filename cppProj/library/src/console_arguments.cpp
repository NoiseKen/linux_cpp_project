#include <stdexcept>
#include "extension_string.h"
#include "console_arguments.h"
#include "sys_ctrl.h"
#include "file_ctrl.h"
std::string ConsoleArguments::appRoot;
std::string ConsoleArguments::appName;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
std::string
ConsoleArguments::var_to_filename(int idx, bool &exist)
{
    std::string xferFileName;
    if(vars[idx].enabled && !vars[idx].value().empty())
    {
        std::string head="";
        bool refPath=true;
        if(vars[idx].value().length()>1)
        {
            head=std::string(vars[idx].value().c_str(), 1);
            if(head=="/")
            {
                refPath=false;
            }

        }

        if(refPath)
        {//related path
            xferFileName = ConsoleArguments::appRoot + "/" + vars[idx].value();
        }
        else
        {
            xferFileName = vars[idx].value();
        }
        FileCtrl *fr;
        fr = new FileCtrl(xferFileName);
        exist = fr->is_exist();        
        delete fr;
    }
    else
    {
        exist=false;
        xferFileName="";
    }
    return xferFileName;   
}
//---------------------------------------------------------------------------
std::string
ConsoleArguments::valid_io_in_file_name(int idx, bool &valid, bool acceptEmpty)
{
    std::string xferFileName;
    bool exist;
    xferFileName = var_to_filename(idx, exist);

    if(!acceptEmpty&&xferFileName.empty())
    {
        printf("[%s] argument can not be empty!!\n", vars[idx].value().c_str());
        valid=false;
        //force reutrn condition
        exist=true;
    }

    if(!exist&&!xferFileName.empty())
    {
        //std::string msg;
        //FileRecord *fr;
        //fr = new FileRecord(xferFileName);
        //exist = fr->create(true, msg);
        FileCtrl *fr;
        fr = new FileCtrl(xferFileName);
        exist = (0==fr->create())?true:false;
        if(!exist)
        {//can NOT create read back file
            printf("Xfer In file [%s] cannot be created!!\n", xferFileName.c_str());
            //printf("%s\n", msg.c_str());
            valid=false;
        }
        else
        {//if folder hierarchy is valid, remove file just created, avoid this vu command unsuccessful
            fr->destory();
        }
        delete fr;
    }
    return xferFileName;
}
//---------------------------------------------------------------------------
std::string
ConsoleArguments::valid_io_out_file_name(int idx, uint32_t requiredByteCount, bool &valid)
{
    std::string xferFileName;

    xferFileName = var_to_filename(idx, valid);
    if(!valid)
    {
        printf("Xfer Out file [%s] can not found!!\n", xferFileName.c_str());
    }
    else
    {//check file size
        FileCtrl *fr;
        uint64_t size;
        fr = new FileCtrl(xferFileName);
        size=fr->file_size();
        if(size<requiredByteCount)
        {
            valid=false;
            printf("Xfer Out file [%s] size not meet required!!\n", xferFileName.c_str());
            printf("file size [%ld] < required [%d]\n", size, requiredByteCount);
        }
        delete fr;
    }

    return xferFileName;
}
//---------------------------------------------------------------------------
uint32_t
ConsoleArguments::var_to_int32(int idx, bool &valid)
{
    uint32_t to=0;
    bool lvalid=true;
    to=SysCtrl::string_to_int32(vars[idx].value(), lvalid);
    if(!lvalid)
    {
        printf("Argument [%s] give invalid int32 value [%s]!!\n", vars[idx].keyWord.c_str(), vars[idx].value().c_str());
        valid=false;        
    }
    return to;
}
//---------------------------------------------------------------------------
uint64_t
ConsoleArguments::var_to_int64(int idx, bool &valid)
{
    uint64_t to=0;
    bool lvalid=true;
    to=SysCtrl::string_to_int64(vars[idx].value(), lvalid);
    if(!lvalid)
    {
        printf("Argument [%s] give invalid int64 value [%s]!!\n", vars[idx].keyWord.c_str(), vars[idx].value().c_str());
        valid=false;        
    }
    return to;
}
//---------------------------------------------------------------------------
void
ConsoleArguments::dump_vars(void)
{
    for(int i=0;i<varsCount;i++)
    {
        printf("var[%d], key=%s, value=%s\n", i, vars[i].keyWord.c_str(), vars[i].value().c_str());
    }
}
//---------------------------------------------------------------------------
void
ConsoleArguments::_argv_parsing(int argc, char* argv[])
{
    if(argc>0)
    {
        ExtensionString *es;
        es = new ExtensionString("=");

        for(int i=1;i<argc;i++)
        {
            es->string=std::string(argv[i]);
            int j = varsCount;
            while(--j>-1)
            {
                if(vars[j].keyWord==(*es)[0])
                {
                    std::string val="";
                    bool first=true;
                    for(int s=1;s<(int)es->size();s++)
                    {
                        if(!first)
                        {
                            val+="=";
                        }
                        val+=(*es)[s];
                        first=false;
                    }
                    vars[j].set_value(val);
                    j=-1;
                }
            }
        }
        delete es;
    }
}
//---------------------------------------------------------------------------
void
ConsoleArguments::show_var_help(std::string title)
{
    std::string val;
    printf("%s\n", title.c_str());

    for(int i=0;i<varsCount;i++)
    {
        if(vars[i].hide)
        {
            continue;
        }

        val = vars[i].value();
        if((val.length()==0)||(val==SW_DEFAULT))
        {
            printf("%s, %s\n", vars[i].keyWord.c_str(), vars[i].help.c_str());
        }
        else
        {
            printf("%s [def=%s], %s\n", vars[i].keyWord.c_str(), val.c_str(), vars[i].help.c_str());
        }
    }
}
//---------------------------------------------------------------------------
ConsoleArguments::ConsoleArguments(int argc, char* argv[], GlobalVariable * const pVars, int varsSize)
    : vars(pVars), varsCount(varsSize/sizeof(GlobalVariable))
{
    _argv_parsing(argc, argv);
}
//---------------------------------------------------------------------------
ConsoleArguments::~ConsoleArguments(void)
{
}
//---------------------------------------------------------------------------
