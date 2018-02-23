#ifndef _GLOBAL_VARIABLE_H_
#define _GLOBAL_VARIABLE_H_
#include <string>
#include "property.h"

#define SW_DEFAULT  "DeAdC0De*%^^&POPO"

#define HideSwitch(x) GlobalVariable(x, "", SW_DEFAULT, true)

class GlobalVariable
{
public:
    std::string const keyWord;
    std::string const help;
    bool const hide;
    void set_value(std::string in);
    std::string value(void);
    property_rw<GlobalVariable, bool> enabled; //={read=fGetRawSwitch, write=fSetRawSwitch};
    GlobalVariable(std::string key, std::string desc, std::string def=SW_DEFAULT, bool hide=false);
    ~GlobalVariable(void);
protected:
    std::string _raw;
private:
    void fSetRawSwitch(bool en);
    bool fGetRawSwitch(void);
};

#endif //ndef _GLOBAL_VARIABLE_H_
