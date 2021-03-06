#include "global_variable.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
GlobalVariable::fGetRawSwitch(void)
{
    return (_raw==SW_DEFAULT)?false:true;
}
//---------------------------------------------------------------------------
void
GlobalVariable::fSetRawSwitch(bool en)
{
    _raw = std::string((en)?"":SW_DEFAULT);
}
//---------------------------------------------------------------------------
std::string 
GlobalVariable::value(void)
{
    return _raw;
}
//---------------------------------------------------------------------------
void 
GlobalVariable::set_value(std::string in)
{
    _raw = in;
}
//---------------------------------------------------------------------------
GlobalVariable::GlobalVariable(std::string key, std::string desc, std::string def, bool hide)
    : keyWord(key), help(desc), hide(hide)
    , enabled(this, &GlobalVariable::fGetRawSwitch, &GlobalVariable::fSetRawSwitch)
{
    _raw = def;
}
//---------------------------------------------------------------------------
GlobalVariable::~GlobalVariable(void)
{
}
//---------------------------------------------------------------------------
