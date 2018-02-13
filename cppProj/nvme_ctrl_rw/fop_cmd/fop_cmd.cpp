#include "fop_cmd.h"
//#include "file_record.h"
#include "memory.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
FOPCmd::post_ds_to_file(int idx, uint32_t byteCount, void *buf)
{
    std::string xferFileName;
    bool isToFile;
    xferFileName = var_to_filename(idx, isToFile);

    isToFile=xferFileName.empty()?false:true;

    if(isToFile)
    {//have assigned file name to store data
        #if 0
        TMemoryStream *ms;
        ms = new TMemoryStream();
        ms->Write(buf, byteCount);
        ms->SaveToFile(xferFileName);
        printf("save data structure to file [%s]\n", xferFileName);
        delete ms;
        #else
        Memory<uint32_t> *memSave;
        memSave=new Memory<uint32_t>((uint32_t *)buf);
        int sc;
        sc=memSave->save_to(xferFileName, byteCount);
        printf("save data structure to file [%s]\n", xferFileName.c_str());
        if(sc!=0)
        {
            printf("save data to file fail!!\n");
        }
        delete memSave;
        #endif
    }

    return isToFile;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
FOPCmd::FOPCmd(int argc, char* argv[], std::string cmdStr, GlobalVariable *const pVars, int varsSize)
    : ConsoleArguments(argc, argv, pVars, varsSize), cmdString(cmdStr)
{
}
//---------------------------------------------------------------------------
FOPCmd::~FOPCmd(void)
{
}
//---------------------------------------------------------------------------
